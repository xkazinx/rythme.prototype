#include "pch.h"
#include "CommandMgr.h"
#include "App.h"

CommandMgr::CommandMgr()
	: MgrBase()
{
	Setup();
}

void CommandMgr::Setup()
{
	_list.clear();

	using This = CommandMgr;

	_list =
	{
		Cmd
		{ "h",			&This::Help			},
		{ "out",			&This::Output		},
	};
	
	SetAlias("help",		"h");
}

bool CommandMgr::Handle()
{
	if(std::invoke(GetCommand()->_func, this))
	{
		return true;
	}

	return false;
}

command::Command<CommandMgr> * CommandMgr::Process(const string & str)
{
	return static_cast<command::MgrBase<CommandMgr> &>(*g_cmd.get()).Handle(str);
}

bool CommandMgr::Help()
{
	return true;
}

bool CommandMgr::Output()
{
	g_app->ToggleConsole();
	return true;
}
