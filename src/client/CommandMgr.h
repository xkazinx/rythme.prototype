#pragma once
#include "CommandBase.h"

class CommandMgr : public command::MgrBase<CommandMgr>
{
public:
	using Flag = command::Flag;

	CommandMgr();
	void Setup();
	~CommandMgr() override = default;

	bool Handle() override;

	static command::Command<CommandMgr> * Process(const string & str);

public:
	bool Help();
	bool Output();
};
