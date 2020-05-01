#include "RequestParser.h"
#include <iostream>

#define PACKTOSTRING_BEGIN(COMMAND_NAME) \
	tinyxml2::XMLDocument doc;\
	tinyxml2::XMLElement* root = doc.NewElement("request");\
	doc.LinkEndChild(root);\
	root->SetAttribute("command", (COMMAND_NAME));\
	tinyxml2::XMLElement* data = root->InsertNewChildElement("command_data");

#define PACKTOSTRING_END \
	tinyxml2::XMLPrinter printer;\
	doc.Print(&printer);\
	return printer.CStr();

//--------------------------------------------------------
bool IRequestImpl::UnpackFromXML(tinyxml2::XMLDocument & doc)
{
	throw std::exception("this method is not implemented.");
}

REQUEST_ID IRequestImpl::GetCommandID()
{
	throw std::exception("this method is not implemented.");
}

std::string IRequestImpl::GetPlayerName()
{
	throw std::exception("this method is not implemented.");
}

int IRequestImpl::GetPlayerID()
{
	throw std::exception("this method is not implemented.");
}

int IRequestImpl::GetGameID()
{
	throw std::exception("this method is not implemented.");
}

std::string IRequestImpl::PackToString()
{
	throw std::exception("this method is not implemented.");
}

PLAYER_LEVEL IRequestImpl::GetLevel()
{
	throw std::exception("this method is not implemented.");
}

PLAYER_LEVEL IRequestImpl::GetOpponentLevel()
{
	throw std::exception("this method is not implemented.");
}

std::pair<int, int> IRequestImpl::GetShotPos()
{
	throw std::exception("this method is not implemented.");
}
//--------------------------------------------------------
REQUEST_ID ErrorRequest::GetCommandID()
{
	return REQUEST_ID::RQ_ERROR;
}

bool ErrorRequest::UnpackFromXML(tinyxml2::XMLDocument& doc)
{
	return true;
}
//--------------------------------------------------------
RegisterRequest::RegisterRequest()
	:m_player("")
	, m_my_level(PLAYER_LEVEL::PL_ERROR)
{
}

RegisterRequest::RegisterRequest(const std::string& player, PLAYER_LEVEL my_level)
:m_player(player)
, m_my_level(my_level)
{
}

REQUEST_ID RegisterRequest::GetCommandID()
{
	return REQUEST_ID::RQ_REGISTER;
}

std::string RegisterRequest::GetPlayerName()
{
	return m_player;
}
PLAYER_LEVEL RegisterRequest::GetLevel()
{
	return m_my_level;
}

std::string RegisterRequest::PackToString()
{
	PACKTOSTRING_BEGIN("REGISTER")
	data->SetAttribute("player_name", m_player.c_str());
	data->SetAttribute("player_level", (int)m_my_level);
	PACKTOSTRING_END
}

bool RegisterRequest::UnpackFromXML(tinyxml2::XMLDocument& doc)
{
	tinyxml2::XMLElement* p_data = doc.RootElement()->FirstChildElement("command_data");
	if (p_data)
	{
		m_player = p_data->Attribute("player_name");
		m_my_level = (PLAYER_LEVEL)p_data->IntAttribute("player_level");
		return true;
	}
	return false;
}
//--------------------------------------------------------
StartGameRequest::StartGameRequest()
	:m_player_id(-1)
	, m_opponent_level(PLAYER_LEVEL::PL_ERROR)
{}

StartGameRequest::StartGameRequest(int player_id, PLAYER_LEVEL opponent_level)
	:m_player_id(player_id)
	, m_opponent_level(opponent_level)
{}

REQUEST_ID StartGameRequest::GetCommandID()
{
	return REQUEST_ID::RQ_START_GAME;
}

int StartGameRequest::GetPlayerID()
{
	return m_player_id;
}

std::string StartGameRequest::PackToString()
{
	PACKTOSTRING_BEGIN("START_GAME")
	data->SetAttribute("player_id", m_player_id);
	data->SetAttribute("opponent_level", (int)m_opponent_level);
	PACKTOSTRING_END
}

bool StartGameRequest::UnpackFromXML(tinyxml2::XMLDocument& doc)
{
	tinyxml2::XMLElement* p_data = doc.RootElement()->FirstChildElement("command_data");
	if (p_data)
	{
		m_player_id = p_data->IntAttribute("player_id");
		m_opponent_level = (PLAYER_LEVEL)p_data->IntAttribute("opponent_level");
		return true;
	}
	return false;
}

PLAYER_LEVEL StartGameRequest::GetOpponentLevel()
{
	return m_opponent_level;
}

//--------------------------------------------------------
GetFieldRequest::GetFieldRequest()
	:m_player_id(-1)
	, m_game_id(-1)
{}

GetFieldRequest::GetFieldRequest(int player_id, int game_id)
:m_player_id(player_id)
, m_game_id(game_id)
{}

REQUEST_ID GetFieldRequest::GetCommandID()
{
	return REQUEST_ID::RQ_GET_FIELD;
}

int GetFieldRequest::GetPlayerID()
{
	return m_player_id;
}

int GetFieldRequest::GetGameID()
{
	return m_game_id;
}

std::string GetFieldRequest::PackToString()
{
	PACKTOSTRING_BEGIN("GET_FIELD")
	data->SetAttribute("player_id", m_player_id);
	data->SetAttribute("game_id", m_game_id);
	PACKTOSTRING_END
}

bool GetFieldRequest::UnpackFromXML(tinyxml2::XMLDocument& doc)
{
	tinyxml2::XMLElement* p_data = doc.RootElement()->FirstChildElement("command_data");
	if (p_data)
	{
		m_player_id = p_data->IntAttribute("player_id");
		m_game_id = p_data->IntAttribute("game_id");
		return true;
	}
	return false;
}
//--------------------------------------------------------
MakeShotRequest::MakeShotRequest()
	:m_player_id(-1)
	, m_game_id(-1)
	, m_ver(-1)
	, m_hor(-1)
{}

MakeShotRequest::MakeShotRequest(int player_id, int game_id, int ver, int hor)
	:m_player_id(player_id)
	, m_game_id(game_id)
	, m_ver(ver)
	, m_hor(hor)
{}


REQUEST_ID MakeShotRequest::GetCommandID()
{
	return REQUEST_ID::RQ_MAKE_SHOT;
}

int MakeShotRequest::GetPlayerID()
{
	return m_player_id;
}

int MakeShotRequest::GetGameID()
{
	return m_game_id;
}

std::string MakeShotRequest::PackToString()
{
	PACKTOSTRING_BEGIN("MAKE_SHOT")
	data->SetAttribute("player_id", m_player_id);
	data->SetAttribute("game_id", m_game_id);
	data->SetAttribute("shot_ver", m_ver);
	data->SetAttribute("shot_hor", m_hor);
	PACKTOSTRING_END
}

bool MakeShotRequest::UnpackFromXML(tinyxml2::XMLDocument& doc)
{
	tinyxml2::XMLElement* p_data = doc.RootElement()->FirstChildElement("command_data");
	if (p_data)
	{
		m_player_id = p_data->IntAttribute("player_id");
		m_game_id = p_data->IntAttribute("game_id");
		m_ver = p_data->IntAttribute("shot_ver");
		m_hor = p_data->IntAttribute("shot_hor");
		return true;
	}
	return false;
}

std::pair<int, int> MakeShotRequest::GetShotPos()
{
	return std::make_pair(m_ver, m_hor);
}
//--------------------------------------------------------

//--------------------------------------------------------
IRequestPtr RequestParser::Parse(const std::string& str)
{
	tinyxml2::XMLDocument doc;
	if (doc.Parse(str.c_str()) == tinyxml2::XML_SUCCESS)
	{
		IRequestPtr p_res;
		const char* cmd = doc.RootElement()->Attribute("command");
		if (cmd && std::string(cmd) == "REGISTER")
			p_res = std::make_shared<RegisterRequest>();
		else if (cmd && std::string(cmd) == "START_GAME")
			p_res = std::make_shared<StartGameRequest>();
		else if (cmd && std::string(cmd) == "GET_FIELD")
			p_res = std::make_shared<GetFieldRequest>();
		else if (cmd && std::string(cmd) == "MAKE_SHOT")
			p_res = std::make_shared<MakeShotRequest>();
		else
			p_res = std::make_shared<ErrorRequest>();
		p_res->UnpackFromXML(doc);
		return p_res;
	}
	return std::make_shared<ErrorRequest>();
}

std::string RequestParser::Pack(IRequestPtr command)
{
	return command->PackToString();
}

#ifdef _TEST
namespace test
{
	bool __test1(void)
	{
		IRequestPtr rq = std::make_shared<RegisterRequest>("Vasia", PLAYER_LEVEL::PL_HUMAN);
		std::string str = RequestParser::Pack(rq);
		IRequestPtr rq2 = RequestParser::Parse(str);
		return (rq->GetPlayerName() == rq2->GetPlayerName()) && (rq->GetLevel() == rq2->GetLevel());
	}
	bool __test2(void)
	{
		IRequestPtr rq = std::make_shared<StartGameRequest>(1, PLAYER_LEVEL::PL_ADVANCED);
		std::string str = RequestParser::Pack(rq);
		IRequestPtr rq2 = RequestParser::Parse(str);
		return (rq->GetPlayerID() == rq2->GetPlayerID()) && (rq->GetOpponentLevel() == rq2->GetOpponentLevel());
	}
	bool __test3(void)
	{
		IRequestPtr rq = std::make_shared<GetFieldRequest>(17, 33);
		std::string str = RequestParser::Pack(rq);
		IRequestPtr rq2 = RequestParser::Parse(str);
		return (rq->GetPlayerID() == rq2->GetPlayerID()) && (rq->GetGameID() == rq2->GetGameID());
	}
	bool __test4(void)
	{
		IRequestPtr rq = std::make_shared<MakeShotRequest>(17, 33, 4, 8);
		std::string str = RequestParser::Pack(rq);
		IRequestPtr rq2 = RequestParser::Parse(str);
		return (rq->GetPlayerID() == rq2->GetPlayerID()) && (rq->GetGameID() == rq2->GetGameID()) &&
			(rq->GetShotPos() == rq2->GetShotPos());
	}
	bool __test5(void)
	{
		std::string str = "gfsfgsdfg";
		IRequestPtr rq = RequestParser::Parse(str);
		return (rq->GetCommandID() == REQUEST_ID::RQ_ERROR);
	}
	int all_tests()
	{
		std::cout << "test1 is " << (__test1() ? "PASS" : "FAILED") << std::endl;
		std::cout << "test2 is " << (__test2() ? "PASS" : "FAILED") << std::endl;
		std::cout << "test3 is " << (__test3() ? "PASS" : "FAILED") << std::endl;
		std::cout << "test4 is " << (__test4() ? "PASS" : "FAILED") << std::endl;
		std::cout << "test5 is " << (__test5() ? "PASS" : "FAILED") << std::endl;
		return 0;
	}
	int r=all_tests();
}
#endif
