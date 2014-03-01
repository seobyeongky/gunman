#pragma once

#include "internal.h"


namespace NetInterface
{
	typedef function<void(const vector<server_info_t>)> svinfo_cb_t;

	void Begin();
	void End();

	bool Connect(const wstring & addr, const wstring & user_name);

	void Update();

	function<void()> RegisterConnectFailedCallback(function<void()> callback);
	function<void()> RegisterConnectedCallback(function<void(const server_info_t & sv_info, ID my_id)> callback);
	function<void()> RegisterClientIntroCallback(function<void(const client_t & cl_info)> callback);
	function<void()> RegisterClientGoneCallback(function<void(const client_t & cl_info)> callback);
	function<void()> RegisterPacketCallback(sv_to_cl_t type, function<void(Packet & packet)> callback);
	function<void()> RegisterServerListUpdate(svinfo_cb_t callback);
	void ClearCallbacks();
	void GetClients(smap<ID, client_t> * ptr);
}
