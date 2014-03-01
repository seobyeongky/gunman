#include "asset.h"
#include "util.h"
#include "global.h"

#include <ole2.h>
#include <xmllite.h>
#include <stdio.h>
#include <shlwapi.h>

struct xmlElement_t
{
	wstring							name;
	list<pair<wstring, wstring>>	attr;
};

enum xmlMsgType_t
{
	XML_MSG_ELEMENT,
	XML_MSG_ELEMENT_END
};

struct xmlMsg_t
{
	xmlMsgType_t	type;
	xmlElement_t	element;
};

class XmlFile
{
public:
	XmlFile(LPCWSTR image_data_file)
		: file_stream(nullptr),
		reader(nullptr)
	{
		HRESULT hr;
		//Open read-only input stream
		if (FAILED(hr = SHCreateStreamOnFile(image_data_file, STGM_READ, &file_stream)))
		{
			G.logger->Error(L"Error creating file reader");
			return;
		}

		if (FAILED(hr = CreateXmlReader(__uuidof(IXmlReader), (void**) &reader, NULL)))
		{
			G.logger->Error(L"Error creating xml reader");
			return;
		}

		if (FAILED(hr = reader->SetProperty(XmlReaderProperty_DtdProcessing, DtdProcessing_Prohibit)))
		{
			G.logger->Error(L"Error setting XmlReaderProperty_DtdProcessing");
			return;
		}

		if (FAILED(hr = reader->SetInput(file_stream)))
		{
			G.logger->Error(L"Error setting input for reader");
			return;
		}
	}
	~XmlFile()
	{
		if(file_stream != nullptr) file_stream->Release();
		if(reader != nullptr) reader->Release();
	}

	bool Parse(xmlMsg_t *msg)
	{
		HRESULT			hr;
		XmlNodeType		node_type;
		LPCWSTR			pwszLocalName = nullptr;
		LPCWSTR			pwszValue = nullptr;

RETRY:
		msg->element.attr.clear();

		hr = reader->Read(&node_type);
		if(hr != S_OK)
			return false;

		switch (node_type)
        {
        case XmlNodeType_Element:
			msg->type = XML_MSG_ELEMENT;
            if (FAILED(hr = reader->GetLocalName(&pwszLocalName, NULL)))
            {
				G.logger->Error(L"Error getting local name");
				return false;
            }
			msg->element.name = pwszLocalName;
			std::transform(msg->element.name.begin(), msg->element.name.end(),msg->element.name.begin(), ::tolower);

			hr = reader->MoveToFirstAttribute();
			if(hr == S_FALSE)
				break;
			if(hr != S_OK)
			{
				G.logger->Error(L"Error moving to first attribute");
				return false;
			}
			else
			{
				while(TRUE)
				{
					if(!reader->IsDefault())
					{
		                if (FAILED(hr = reader->GetLocalName(&pwszLocalName, NULL)))
						{
							G.logger->Error(L"Error getting local name");
							return false;
						}
		                if (FAILED(hr = reader->GetValue(&pwszValue, NULL)))
		                {
							G.logger->Error(L"Error getting value");
							return false;
						}
						pair<wstring, wstring> pair(pwszLocalName, pwszValue);
						std::transform(pair.first.begin(), pair.first.end(), pair.first.begin(), ::tolower);
						std::transform(pair.second.begin(), pair.second.end(), pair.second.begin(), ::tolower);
						msg->element.attr.push_back(pair);
					}
					hr = reader->MoveToNextAttribute();
					if(hr == S_FALSE)
						break;
					else if(hr != S_OK)
					{
						G.logger->Error(L"Error moving to next attribute");
						return false;
					}
				}
			}
            break;

        case XmlNodeType_EndElement:
			msg->type = XML_MSG_ELEMENT_END;
            if (FAILED(hr = reader->GetLocalName(&pwszLocalName, NULL)))
            {
				G.logger->Error(L"Error getting local name");
				return false;
            }

			msg->element.name = pwszLocalName;
			std::transform(msg->element.name.begin(), msg->element.name.end(),msg->element.name.begin(), ::tolower);
			break;

		default:
			goto RETRY;
        }

		return true;
	}

private:
	IStream		*file_stream;
	IXmlReader	*reader;
};

bool LoadSpriteFromXml(LPCWSTR xmlfile)
{
	Texture		*texture = nullptr;
	xmlMsg_t	msg;
	bool		imageset_inside = false;
	XmlFile		file(xmlfile);

	while(file.Parse(&msg))
	{
		switch(msg.type)
		{
		case XML_MSG_ELEMENT:
			if(imageset_inside)
			{
				if(msg.element.name != L"image")
				{
					G.logger->Error(L"imageset�ȿ� image�� �ƴ� ���� �ֽ��ϴ�. (%s)", msg.element.name.c_str());
					return false;
				}
				wstring name;
				
				int width, height, x_pos, y_pos;
				int set = 0;

				for(auto i = msg.element.attr.begin(); i != msg.element.attr.end(); ++i)
				{
					if(i->first == L"name")
					{
						name = i->second;
						assert((set & 0x001) == 0);
						set += 0x001;
					}
					else if(i->first == L"width")
					{
						width =  _wtoi(i->second.c_str());
						assert((set & 0x002) == 0);
						set += 0x002;
					}
					else if(i->first == L"height")
					{
						height = _wtoi(i->second.c_str());
						assert((set & 0x004) == 0);
						set += 0x004;
					}
					else if(i->first == L"xpos")
					{
						x_pos = _wtoi(i->second.c_str());
						assert((set & 0x008) == 0);
						set += 0x008;
					}
					else if(i->first == L"ypos")
					{
						y_pos = _wtoi(i->second.c_str());
						assert((set & 0x010) == 0);
						set += 0x010;
					}
				}

				if((set & 0x01f) == 0x01f)
				{
					G.sprite_map[name] = new Sprite(*texture, IntRect(x_pos, y_pos, width, height));
				}
				else
				{
					G.logger->Error(L"Texture ���� file ����");
					return false;
				}
			}
			else
			{
				if(msg.element.name != L"imageset")
				{
					G.logger->Error(L"Imageset�� �ƴ� ���� �ֽ��ϴ�. (%s)", msg.element.name.c_str());
					return false;
				}
				for(auto i = msg.element.attr.begin(); i != msg.element.attr.end(); ++i)
				{
					if(i->first == L"imagefile")
					{
						smap<wstring, Texture *>::Iter iter;
						const wstring & texture_file_name = i->second;

						if(!G.texture_map.find(texture_file_name))
						{
							//���� �ؽ��� ����Ʈ�� ���� ������ ����Դϴ�.
							texture = new Texture();
							string ansi_file_name(texture_file_name.begin(), texture_file_name.end());
							if(!texture->loadFromFile(ansi_file_name))
							{
								G.logger->Error(L"Texture file load ���� (%s)", texture_file_name.c_str());
								return false;
							} 
							G.texture_map.insert(texture_file_name, texture);
						}
						break;
					}
				}
				if(texture == nullptr)
				{
					G.logger->Error(L"Imageset���� Texture file ������ ã�� ���߽��ϴ�.");
					return false;
				}
				imageset_inside = true;
			}
			break;

		case XML_MSG_ELEMENT_END:
			imageset_inside = false;
			break;
		}
	}
	return true;
}

bool LoadSprite(LPCWSTR image_dir)
{
	DirChanger		dir_changer(image_dir);
	vector<wstring>	list;

	GetMatchedFileList(&list, L"*.xml");
	for (auto & filename : list)
	{
		if (!LoadSpriteFromXml(filename.c_str())) return false;
	}

	return true;
}

bool LoadSystemAssets()
{
	if (!G.default_font.loadFromFile("data\\system\\font\\�������Bold.ttf"))
	{
		G.logger->Error(L"LoadSystemAssets : default font load ����");
		return false;
	}

	if (!G.title_font.loadFromFile("data\\system\\font\\a���������B.ttf"))
	{
		G.logger->Error(L"LoadSystemAssets : title font load ����");
		return false;
	}

	if (!LoadSprite(L"data\\system\\sprite\\"))
	{
		G.logger->Error(L"LoadAssets: image load ����");
		return false;
	}
	
	if (G.bg_music = audiere::OpenSound(G.audio_device, "data\\system\\audio\\TAM-G01.mp3", true))
	{
		G.bg_music->setRepeat(true);
	}

	return true;
}

void ReleaseAssets()
{
	for(auto it = G.sprite_map.begin(); it != G.sprite_map.end(); ++it)
		delete (*it).element();
	G.sprite_map.clear();

	for(auto iter = G.texture_map.begin(); iter != G.texture_map.end(); ++iter)
		delete (*iter).element();
	G.texture_map.clear();
}