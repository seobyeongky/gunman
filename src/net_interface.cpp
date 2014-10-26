#include "net_interface.h"
#include "internal.h"
#include "global.h"

namespace NetInterface
{
	typedef function<void(client_msg_t & msg)> callback_t;
	typedef smap<client_msg_type_t, callback_t> callback_map_t;
	typedef smap<sv_to_cl_t, function<void(Packet & packet)>> packet_callback_map_t;
	typedef smap<int, svinfo_cb_t> svlist_cb_map_t;

	bool					began = false;
	vector<server_info_t>	found_sv_list;
	callback_map_t			callback_map;
	packet_callback_map_t	packet_callback_map;
	svlist_cb_map_t			svlist_cb_map;
	int						svlist_cb_id_count = 0;
	client_msg_t			cl_msg;
	smap<ID, client_t>		client_map;

	void Begin()
	{
		BeginClient();
		began = true;
		if (!RequestSearchLocalNetServers())
			G.logger->Error(L"서버 찾기 서비스 시작 실패");
	}

	void End()
	{
		began = false;
		EndClient();
	}

	bool Connect(const wstring & addr, const wstring & user_name)
	{
		return RequestConnectToServer(addr.c_str(), user_name.c_str());
	}

	void Update()
	{
		while (GetClientMsg(&cl_msg))
		{
			switch (cl_msg.type)
			{
			case CLMSG_CONNECT_FAILED:
			case CLMSG_CONNECTED:
			case CLMSG_DISCONNECTED:
			case CLMSG_CLIENT_INTRODUCTION:
			case CLMSG_CLIENT_GONE:
				{
					switch (cl_msg.type)
					{
					case CLMSG_CLIENT_INTRODUCTION:
						client_map[cl_msg.client_info.id] = cl_msg.client_info;
						break;
					case CLMSG_CLIENT_GONE:
						client_map.erase(cl_msg.client_info.id);
						break;
					}

					callback_map_t::Iter iter;
					if (callback_map.find(cl_msg.type, &iter))
					{
						(*iter).element()(cl_msg);
					}
				}
				break;
			case CLMSG_LOCAL_SERVER_FOUND:
				found_sv_list.push_back(cl_msg.server_info);
				break;
			case CLMSG_FINDING_LOCAL_SERVER_OVER:
				for (auto & cb : svlist_cb_map) cb.element()(found_sv_list);
				found_sv_list.clear();
				if (!RequestSearchLocalNetServers())
					G.logger->Error(L"서버 찾기 서비스 잇기 실패");
				break;
			case CLMSG_UNEXPECTED_ERROR:
				G.logger->Warning(L"Unexpected error : %s", cl_msg.error_msg);
				break;
			case CLMSG_PACKET_RECV:
				{
					unsigned short header;
					if (!(cl_msg.packet >> header)) return;
					sv_to_cl_t type = static_cast<sv_to_cl_t>(header);

					packet_callback_map_t::Iter iter;
					if (packet_callback_map.find(type, &iter))
					{
						(*iter).element()(cl_msg.packet);
					}
				}
				break;
			}
		}
	}

	function<void()> RegisterConnectFailedCallback(function<void()> callback)
	{
		callback_map[CLMSG_CONNECT_FAILED] = [callback](client_msg_t & msg)
		{
			callback();
		};
		return [](){callback_map.erase(CLMSG_CONNECT_FAILED);};
	}

	function<void()> RegisterConnectedCallback(function<void(const server_info_t & sv_info, ID my_id)> callback)
	{
		callback_map[CLMSG_CONNECTED] = [callback](client_msg_t & msg)
		{
			callback(msg.server_info, msg.my_id);
		};
		return [](){callback_map.erase(CLMSG_CONNECTED);};
	}

	function<void()> RegisterClientIntroCallback(function<void(const client_t & cl_info)> callback)
	{
		callback_map[CLMSG_CLIENT_INTRODUCTION] = [callback](client_msg_t & msg)
		{
			callback(msg.client_info);
		};
		return [](){callback_map.erase(CLMSG_CLIENT_INTRODUCTION);};
	}

	function<void()> RegisterClientGoneCallback(function<void(const client_t & cl_info)> callback)
	{
		callback_map[CLMSG_CLIENT_GONE] = [callback](client_msg_t & msg)
		{
			callback(msg.client_info);
		};
		return [](){callback_map.erase(CLMSG_CLIENT_GONE);};
	}
	
	function<void()> RegisterPacketCallback(sv_to_cl_t type, function<void(Packet & packet)> callback)
	{
		packet_callback_map[type] = [callback](Packet & packet)
		{
			callback(packet);
		};
		return [type](){packet_callback_map.erase(type);};
	}

	function<void()> RegisterServerListUpdate(svinfo_cb_t callback)
	{
		callback(found_sv_list);

		int id = svlist_cb_id_count++;
		svlist_cb_map[id] = callback;
		return [id](){svlist_cb_map.erase(id);};
	}

	void ClearCallbacks()
	{
		callback_map.clear();
		packet_callback_map.clear();
	}

	void GetClients(smap<ID, client_t> * ptr)
	{
		*ptr = client_map;
	}
};