#include "ResponseParser.h"
#include <iostream>

#define PACKTOSTRING_BEGIN(COMMAND_NAME) \
	tinyxml2::XMLDocument doc;\
	tinyxml2::XMLElement* root = doc.NewElement("response");\
	doc.LinkEndChild(root);\
	root->SetAttribute("command", (COMMAND_NAME));\
	tinyxml2::XMLElement* data = root->InsertNewChildElement("command_data");

#define PACKTOSTRING_END \
	tinyxml2::XMLPrinter printer;\
	doc.Print(&printer);\
	return printer.CStr();

//--------------------------------------------------------

Response_ID IResponseImpl::GetCommandID()
{
	throw std::exception("this method is not implemented.");
}

int IResponseImpl::GetPlayerID()
{
	throw std::exception("this method is not implemented.");
}

int IResponseImpl::GetGameID()
{
	throw std::exception("this method is not implemented.");
}

std::string IResponseImpl::GetField()
{
	throw std::exception("this method is not implemented.");
}

bool IResponseImpl::IsMyTurn()
{
	throw std::exception("this method is not implemented.");
}

AttackResult IResponseImpl::GetAttackResult()
{
	throw std::exception("this method is not implemented.");
}

std::pair<int, int> IResponseImpl::GetShotPos()
{
	throw std::exception("this method is not implemented.");
}

std::pair<bool, int> IResponseImpl::IsFinish()
{
	throw std::exception("this method is not implemented.");
}

std::string IResponseImpl::PackToString()
{
	throw std::exception("this method is not implemented.");
}

bool IResponseImpl::UnpackFromXML(tinyxml2::XMLDocument& doc)
{
	throw std::exception("this method is not implemented.");
}
//--------------------------------------------------------
Response_ID ErrorResponse::GetCommandID()
{
	return Response_ID::RS_ERROR;
}

bool ErrorResponse::UnpackFromXML(tinyxml2::XMLDocument& doc)
{
	return true;
}
//--------------------------------------------------------
RegisterResponse::RegisterResponse()
	:m_player_id(-1)
{
}

RegisterResponse::RegisterResponse(int player_id)
: m_player_id(player_id)
{
}

Response_ID RegisterResponse::GetCommandID()
{
	return Response_ID::RS_REGISTER;
}

int RegisterResponse::GetPlayerID()
{
	return m_player_id;
}

std::string RegisterResponse::PackToString()
{
	PACKTOSTRING_BEGIN("REGISTER")
	data->SetAttribute("player_id", m_player_id);
	PACKTOSTRING_END
}

bool RegisterResponse::UnpackFromXML(tinyxml2::XMLDocument& doc)
{
	tinyxml2::XMLElement* p_data = doc.RootElement()->FirstChildElement("command_data");
	if (p_data)
	{
		m_player_id = p_data->IntAttribute("player_id");
		return true;
	}
	return false;
}
//--------------------------------------------------------
StartGameResponse::StartGameResponse()
	:m_game_id(-1)
{}

StartGameResponse::StartGameResponse(int game_id)
	:m_game_id(game_id)
{}

Response_ID StartGameResponse::GetCommandID()
{
	return Response_ID::RS_START_GAME;
}

int StartGameResponse::GetGameID()
{
	return m_game_id;
}

std::string StartGameResponse::PackToString()
{
	PACKTOSTRING_BEGIN("START_GAME")
	data->SetAttribute("game_id", m_game_id);
	PACKTOSTRING_END
}

bool StartGameResponse::UnpackFromXML(tinyxml2::XMLDocument& doc)
{
	tinyxml2::XMLElement* p_data = doc.RootElement()->FirstChildElement("command_data");
	if (p_data)
	{
		m_game_id = p_data->IntAttribute("game_id");
		return true;
	}
	return false;
}
//--------------------------------------------------------
GetFieldResponse::GetFieldResponse()
	:m_data("")
	, m_is_my_turn(false)
{}

GetFieldResponse::GetFieldResponse(const std::string& data, bool my_turn)
:m_data(data)
, m_is_my_turn(my_turn)
{}

Response_ID GetFieldResponse::GetCommandID()
{
	return Response_ID::RS_GET_FIELD;
}

std::string GetFieldResponse::GetField()
{
	return m_data;
}

bool GetFieldResponse::IsMyTurn()
{
	return m_is_my_turn;
}

std::string GetFieldResponse::PackToString()
{
	PACKTOSTRING_BEGIN("GET_FIELD")
	data->SetAttribute("field", m_data.c_str());
	data->SetAttribute("my_turn", m_is_my_turn);
	PACKTOSTRING_END
}

bool GetFieldResponse::UnpackFromXML(tinyxml2::XMLDocument& doc)
{
	tinyxml2::XMLElement* p_data = doc.RootElement()->FirstChildElement("command_data");
	if (p_data)
	{
		m_data = std::string(p_data->Attribute("field"));
		m_is_my_turn = p_data->BoolAttribute("my_turn");
		return true;
	}
	return false;
}
//--------------------------------------------------------
MakeShotResponse::MakeShotResponse()
	:m_result(AttackResult::AR_ERROR)
	, m_ver(-1)
	, m_hor(-1)
{}

MakeShotResponse::MakeShotResponse(int ver, int hor, AttackResult res)
	:m_result(res)
	, m_ver(ver)
	, m_hor(hor)
{}


Response_ID MakeShotResponse::GetCommandID()
{
	return Response_ID::RS_MAKE_SHOT;
}

std::pair<int, int> MakeShotResponse::GetShotPos()
{
	return std::make_pair(m_ver, m_hor);
}

AttackResult MakeShotResponse::GetAttackResult()
{
	return m_result;
}

bool MakeShotResponse::IsMyTurn()
{
	bool res = false;
	if (m_result == AttackResult::AR_INJURED || m_result == AttackResult::AR_SUNK)
		res = true;
	return res;
}

std::string MakeShotResponse::PackToString()
{
	PACKTOSTRING_BEGIN("MAKE_SHOT")
	data->SetAttribute("shot_ver", m_ver);
	data->SetAttribute("shot_hor", m_hor);
	data->SetAttribute("shot_result", (int)m_result);
	PACKTOSTRING_END
}

bool MakeShotResponse::UnpackFromXML(tinyxml2::XMLDocument& doc)
{
	tinyxml2::XMLElement* p_data = doc.RootElement()->FirstChildElement("command_data");
	if (p_data)
	{
		m_result = (AttackResult)p_data->IntAttribute("shot_result");
		m_ver = p_data->IntAttribute("shot_ver");
		m_hor = p_data->IntAttribute("shot_hor");
		return true;
	}
	return false;
}

//--------------------------------------------------------
UnderAttackResponse::UnderAttackResponse()
	:MakeShotResponse()
{}

UnderAttackResponse::UnderAttackResponse(int ver, int hor, AttackResult res)
	: MakeShotResponse(ver, hor, res)
{}


Response_ID UnderAttackResponse::GetCommandID()
{
	return Response_ID::RS_UNDER_ATTACK;
}

bool UnderAttackResponse::IsMyTurn()
{
	bool res = true;
	if (m_result == AttackResult::AR_INJURED || m_result == AttackResult::AR_SUNK)
		res = false;
	return res;
}

std::string UnderAttackResponse::PackToString()
{
	PACKTOSTRING_BEGIN("UNDER_ATTACK")
	data->SetAttribute("shot_ver", m_ver);
	data->SetAttribute("shot_hor", m_hor);
	data->SetAttribute("shot_result", (int)m_result);
	PACKTOSTRING_END
}
//--------------------------------------------------------
IsFinishResponse::IsFinishResponse()
	:m_is_finish(false)
	, m_winner_id(-1)
{}
IsFinishResponse::IsFinishResponse(bool is_finish, int winner_id)
	: m_is_finish(is_finish)
	, m_winner_id(winner_id)
{}
Response_ID IsFinishResponse::GetCommandID()
{
	return Response_ID::RS_IS_FINISH;
}
std::pair<bool, int> IsFinishResponse::IsFinish()
{
	return std::make_pair(m_is_finish, m_winner_id);
}
std::string IsFinishResponse::PackToString()
{
	PACKTOSTRING_BEGIN("IS_FINISHED")
	data->SetAttribute("is_finish", m_is_finish);
	data->SetAttribute("winner_id", m_winner_id);
	PACKTOSTRING_END
}
bool IsFinishResponse::UnpackFromXML(tinyxml2::XMLDocument& doc)
{
	tinyxml2::XMLElement* p_data = doc.RootElement()->FirstChildElement("command_data");
	if (p_data)
	{
		m_is_finish = p_data->BoolAttribute("is_finish");
		m_winner_id = p_data->IntAttribute("winner_id");
		return true;
	}
	return false;
}
//--------------------------------------------------------
IResponsePtr ResponseParser::Parse(const std::string& str)
{
	tinyxml2::XMLDocument doc;
	if (doc.Parse(str.c_str()) == tinyxml2::XML_SUCCESS)
	{
		IResponsePtr p_res;
		const char* cmd = doc.RootElement()->Attribute("command");
		if (cmd && std::string(cmd) == "REGISTER")
			p_res = std::make_shared<RegisterResponse>();
		else if (cmd && std::string(cmd) == "START_GAME")
			p_res = std::make_shared<StartGameResponse>();
		else if (cmd && std::string(cmd) == "GET_FIELD")
			p_res = std::make_shared<GetFieldResponse>();
		else if (cmd && std::string(cmd) == "MAKE_SHOT")
			p_res = std::make_shared<MakeShotResponse>();
		else if (cmd && std::string(cmd) == "UNDER_ATTACK")
			p_res = std::make_shared<UnderAttackResponse>();
		else if (cmd && std::string(cmd) == "IS_FINISHED")
			p_res = std::make_shared<IsFinishResponse>();
		else
			p_res = std::make_shared<ErrorResponse>();
		p_res->UnpackFromXML(doc);
		return p_res;
	}
	return std::make_shared<ErrorResponse>();
}

std::string ResponseParser::Pack(IResponsePtr command)
{
	return command->PackToString();
}

#ifdef _TEST
namespace ResponseTest
{
	bool __test1(void)
	{
		IResponsePtr rq = std::make_shared<RegisterResponse>(17);
		std::string str = ResponseParser::Pack(rq);
		IResponsePtr rq2 = ResponseParser::Parse(str);
		return (rq->GetPlayerID() == rq2->GetPlayerID());
	}
	bool __test2(void)
	{
		IResponsePtr rq = std::make_shared<StartGameResponse>(77);
		std::string str = ResponseParser::Pack(rq);
		IResponsePtr rq2 = ResponseParser::Parse(str);
		return (rq->GetGameID() == rq2->GetGameID());
	}
	bool __test3(void)
	{
		IResponsePtr rq = std::make_shared<GetFieldResponse>("12,45,34,321,23,45,67,78,89,fg,34,w,34", true);
		std::string str = ResponseParser::Pack(rq);
		IResponsePtr rq2 = ResponseParser::Parse(str);
		return (rq->GetField() == rq2->GetField()) && (rq->IsMyTurn() == rq2->IsMyTurn());
	}
	bool __test4(void)
	{
		IResponsePtr rq = std::make_shared<MakeShotResponse>(4, 8, AttackResult::AR_INJURED);
		std::string str = ResponseParser::Pack(rq);
		IResponsePtr rq2 = ResponseParser::Parse(str);
		return (rq->GetAttackResult() == rq2->GetAttackResult()) && (rq->GetShotPos() == rq2->GetShotPos());
	}
	bool __test5(void)
	{
		IResponsePtr rq = std::make_shared<UnderAttackResponse>(4, 8, AttackResult::AR_INJURED);
		std::string str = ResponseParser::Pack(rq);
		IResponsePtr rq2 = ResponseParser::Parse(str);
		return (rq->GetAttackResult() == rq2->GetAttackResult()) && (rq->GetShotPos() == rq2->GetShotPos());
	}
	bool __test6(void)
	{
		std::string str = "gfsfgsdfg";
		IResponsePtr rq = ResponseParser::Parse(str);
		return (rq->GetCommandID() == Response_ID::RS_ERROR);
	}
	bool __test7(void)
	{
		IResponsePtr rq = std::make_shared<IsFinishResponse>(true, 22);
		std::string str = ResponseParser::Pack(rq);
		IResponsePtr rq2 = ResponseParser::Parse(str);
		return (rq->IsFinish() == rq2->IsFinish());
	}
	int all_tests()
	{
		std::cout << "-------- testing of ResponseParser --------"<< std::endl;
		std::cout << "test1 is " << (__test1() ? "PASS" : "FAILED") << std::endl;
		std::cout << "test2 is " << (__test2() ? "PASS" : "FAILED") << std::endl;
		std::cout << "test3 is " << (__test3() ? "PASS" : "FAILED") << std::endl;
		std::cout << "test4 is " << (__test4() ? "PASS" : "FAILED") << std::endl;
		std::cout << "test5 is " << (__test5() ? "PASS" : "FAILED") << std::endl;
		std::cout << "test6 is " << (__test6() ? "PASS" : "FAILED") << std::endl;
		std::cout << "test7 is " << (__test7() ? "PASS" : "FAILED") << std::endl;
		return 0;
	}
	int r=all_tests();
}
#endif
