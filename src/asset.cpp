#include "asset.h"
#include "util.h"
#include "global.h"

#include <tinyxml.h>
#include <stdio.h>


/*
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
*/

struct img_info_t
{
    std::string name;
    int x, y;
    int width, height;
};

#define CHECK_NULL(name,x) if (!x) {*errmsg = "The Image field " name " missing!"; delete img; return false;}
bool ParseImages(TiXmlElement * parent, vector<img_info_t *> * list_ptr, std::string * errmsg)
{
    for (TiXmlElement * e = parent->FirstChildElement(); e; e = e->NextSiblingElement())
    {
        if (strcmp(e->Value(), "Image") != 0)
        {
            *errmsg = "There is non-Image inside of Imageset";
            return false;
        }

        img_info_t * img = new img_info_t;
        const char * name = e->Attribute("Name");
        CHECK_NULL("Name", name);
        img->name = name;
        CHECK_NULL("XPos", e->Attribute("XPos", &img->x));
        CHECK_NULL("YPos", e->Attribute("YPos", &img->y));
        CHECK_NULL("Width", e->Attribute("Width", &img->width));
        CHECK_NULL("Height", e->Attribute("Height", &img->height));
        list_ptr->push_back(img);
    }
    
    return true;
}

//#define CHECK(cond,themsg...) if (!(cond)) {G.logger->Error(themsg);return false;}

bool LoadSpriteFromXml(const wchar_t * xmlfile)
{
//	xmlMsg_t	msg;
	bool		imageset_inside = false;
    
    TiXmlDocument doc;
    string multi;
    uni2multi(xmlfile, &multi);
    FILE * in = fopen(multi.c_str(), "r");
	if (in == 0) {G.logger->Error(L"failed to open file %s", xmlfile); return false; }
	if (!doc.LoadFile(in)) {G.logger->Error(L"Failed to load %s", xmlfile); return false; }
    fclose(in);
    TiXmlElement* e = doc.FirstChildElement();

	if (e == 0) {G.logger->Error(L"There is no root! %s", xmlfile); return false; }
	if (strcmp(e->Value(), "Imageset") != 0) {G.logger->Error(L"There is a non-Imageset in %s", xmlfile); return false; }
    Texture		*texture = nullptr;
    const char * imgfile = e->Attribute("Imagefile");
	if (imgfile == 0) {G.logger->Error(L"The field Imagefile missing! in %s:%hs", xmlfile, e->Value()); return false;}
    wstring uimgfile;
    multi2uni(imgfile, &uimgfile);
    smap<wstring, Texture *>::Iter it;
    if(G.texture_map.find(uimgfile, &it))
        texture = (*it).element();
    else
    {
        //±‚¡∏ ≈ÿΩ∫√ƒ ∏ÆΩ∫∆Æø° æ¯¥¬ ∆ƒ¿œ¿Œ ∞ÊøÏ¿‘¥œ¥Ÿ.
        texture = new Texture();
		if (!texture->loadFromFile(imgfile)) {G.logger->Error(L"Texture file(%s) load failed", uimgfile.c_str()); return false;}
        G.texture_map.insert(uimgfile, texture);
    }
    
    vector<img_info_t*> list;
    std::string errmsg;
	if (!ParseImages(e, &list, &errmsg)) {G.logger->Error(L"Image parsing failed for file %s : %hs", xmlfile, errmsg.c_str()); return false;}
    
    for (auto img : list)
    {
        wstring theName;
        multi2uni(img->name, &theName);
        G.sprite_map[theName] = new Sprite(*texture, IntRect(img->x, img->y, img->width, img->height));
    }
}

bool LoadSprite(const wchar_t * image_dir)
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
	if (!G.default_font.loadFromFile("data\\system\\font\\나눔고딕Bold.ttf"))
	{
		G.logger->Error(L"LoadSystemAssets : default font load 실패");
		return false;
	}

	if (!G.title_font.loadFromFile("data\\system\\font\\a옛날목욕탕B.ttf"))
	{
		G.logger->Error(L"LoadSystemAssets : title font load 실패");
		return false;
	}

	if (!LoadSprite(L"data\\system\\sprite\\"))
	{
		G.logger->Error(L"LoadAssets: image load 실패");
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