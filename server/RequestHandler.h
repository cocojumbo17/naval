#pragma once
#include <queue>
#include "../common/interfaces.h"
#include "DB.h"

class RequestHandler
{
public:
	RequestHandler(DBPtr db);
	virtual ~RequestHandler();
	void AddRequest(int client_id, IRequestPtr rq);
	void Listen();
	bool Answer(int& o_client_id, IResponsePtr& o_response);
protected:
	void DoRegisterPlayer(int client_id, IRequestPtr rq);
	void DoStartGame(int client_id, IRequestPtr rq);
	void DoGetField(int client_id, IRequestPtr rq);
	void DoMakeShot(int client_id, IRequestPtr rq);

protected:
	std::queue<std::pair<int, IRequestPtr>> m_requests;
	std::queue<std::pair<int, IResponsePtr>> m_responses;
	DBPtr m_db;
};

typedef std::shared_ptr<RequestHandler> RequestHandlerPtr;