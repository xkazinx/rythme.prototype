#pragma once

namespace command
{
using FlagType = u8;

enum Flag : FlagType
{
	None		= 1 << 0,
	//1		= 1 << 1,
	//2		= 1 << 2,
	//3		= 1 << 3,
	//4		= 1 << 4,
	//5		= 1 << 5,
	//6		= 1 << 6,
	//7		= 1 << 7
};

class Parser final
{
public:
	Parser(const char* data, const char* seps = "= \t\n")
		: _data(data), _seps(seps), _data_sz(strlen(data))
	{
	}

	char* Get()
	{
		int i = 0;
		memset(_buffer, 0, kBufferSize);

		while (_cur_idx < _data_sz)
		{
			if (char next_data = _cur_idx <= (_data_sz - 2) ?
				_data[_cur_idx + 1] : 0;
				IsSeparator(_data[_cur_idx], next_data))
			{
				auto flag = false;
				while (!flag)
				{
					next_data = _cur_idx <= (_data_sz - 2) ?
						_data[_cur_idx + 1] : 0;

					if (IsSeparator(_data[_cur_idx], next_data))
					{
						++_cur_idx;
					}
					else
					{
						flag = true;
					}

					if (_cur_idx >= _data_sz - 1)
						flag = true;
				}

				return reinterpret_cast<char *>(&_buffer);
			}
			else
			{
				_buffer[i] = _data[_cur_idx];
				++i;
				++_cur_idx;
			}
		}

		if (strlen(_buffer) == 0)
			return nullptr;

		return reinterpret_cast<char *>(&_buffer);
	}

private:
	const char * _data, * _seps;
	u32 _data_sz, _cur_idx = 0;
	static constexpr szt kBufferSize = 256;
	char _buffer[kBufferSize];

	[[nodiscard]] bool IsSeparator(const char data, const char next_data) const
	{
		if (data == 0 || (data == 0x0D && next_data == 0x0A))
			return true;

		int i = 0;
		while (_seps[i])
		{
			if (_seps[i++] == data)
				return true;
		}

		return false;
	}
};

class Data final
{
public:
	using Idx = u8;

	explicit Data(const string & text)
		: _text(text), _parser(text.c_str())
	{
		auto param = _parser.Get();
		while (param = _parser.Get())
		{
			_data.push_back(param);
		}
	}

	[[nodiscard]] bool Exists(Idx idx) const { return idx < (Idx)_data.size(); }
	[[nodiscard]] bool CanFetch() const { return Exists(_idx); }

	[[nodiscard]] string Get(Idx idx) const { return Exists(idx) ? _data[idx] : ""; }
	[[nodiscard]] bool   Get(Idx idx, string & str) const { return !(str = Get(idx)).empty(); }

	[[nodiscard]] string GetText(Idx starting_idx = 0) const
	{
		if (!Exists(starting_idx))
		{
			Log::Error("GetText: if (!Exists(starting_idx))");
			return "";
		}
		string text;
		for (Idx i = starting_idx; i < static_cast<Idx>(_data.size()) - 1; ++i)
		{
			text += _data[i] + " "; //#check
		}

		text += _data[_data.size() - 1];
		return text;
	}

	template<typename T = long long, typename = std::enable_if_t<std::is_integral_v<T>>>
	[[nodiscard]] T GetValue(Idx idx)
	{
		if (!Exists(idx))
		{
			Log::Error("GetValue: if (!Exists(idx))");
			return 0;
		}

		const auto & str = _data[idx];
		if (!ranges::all_of(str, isdigit))
		{
			if (str.size() <= 1 || str[0] != '-' || !std::all_of(str.begin() + 1, str.end(), isdigit))
				return 0;
		}

		if (str.size() > 10)
			return 0;

		return strtoll(str.c_str(), nullptr, 10);
	}

	template<typename T = string>
	T Pop()
	{
		if constexpr (is_same_v<T, string>)
		{
			return Get(_idx++);
		}
		else if constexpr (is_same_v<T, bool>)
		{
			return GetValue(_idx++) > 0;
		}
		else
		{
			return GetValue<T>(_idx++);
		}
	}

	template<typename T>
	bool Fetch(T & value)
	{
		if (!CanFetch())
		{
			Log::Error("Fetch: if (!CanFetch())");
			return false;
		}

		value = Pop<T>();
		return true;
	}

	template<typename T, typename ... Args>
	bool Fetch(T & value, Args && ...args)
	{
		return Fetch(value) && Fetch(std::forward<Args>(args)...);
	}

private:
	string _text;
	vector<string> _data;
	Parser _parser;
	Idx _idx = 0;
};

template<class Mgr>
class Command final
{
public:
	template<typename Func>
	Command(const string & identifier, Func func, FlagType flags = Flag::None)
		: _func(func), _identifier(identifier), _flags(flags)
	{
	}

	auto operator==(const string & str) const
	{
		return _identifier == str.c_str();
	}

	bool (Mgr::*_func)();
	string      _identifier;
	FlagType    _flags;
};

template<class Mgr>
class Current
{
public:
	Current(Command<Mgr> & cmd, const string & text)
		: _cmd(&cmd), _data(text)
	{
	}

	Current() = default;

	Command<Mgr> * _cmd;
	Data           _data;
};


template<class Mgr>
class MgrBase
{
public:
	using Cmd = Command<Mgr>;

	virtual ~MgrBase<Mgr>() = default;

	// Returns whether the command was ran successfully
	virtual bool Handle() = 0;

	// Returns whether the command was found
	Command<Mgr> * Handle(const string & text)
	{
		constexpr u8 kStartPos = 0;
		if (text.size() < kStartPos || (kStartPos > 0u && text[kStartPos - 1u] != '/'))
			return nullptr;

		const auto first_space = text.find_first_of(" ");
		const auto identifier  = text.substr(kStartPos,
			(first_space == string::npos ? text.size() : first_space)
		);
		
		if (const auto it = find(_list.begin(), _list.end(), identifier);
			it != _list.end())
		{
			_current = make_unique<Current<Mgr>>(*it, text);

			if (!Handle())
			{
				Log::Error("Command {} exists but couldn't be processed due to restrictions or invalid arguments",
				           identifier);
			}

			return _current->_cmd;
		}

		return nullptr;
	}

	unique_ptr<Current<Mgr>> _current;

protected:
	void SetAlias(const string & source, const string & target)
	{
		if (const auto it = find(_list.begin(), _list.end(), source);
			it != _list.end())
		{
			auto cmd_copy        = *it;
			cmd_copy._identifier = target;
			_list.push_back(cmd_copy);
		}
	}

	vector<Command<Mgr>> _list;

	auto GetCaster()
	{
		return _current->_caster;
	}

	auto GetTarget() const
	{
		return _current->_target;
	}

	auto GetCommand() const
	{
		return _current->_cmd;
	}

	auto GetFlag() const
	{
		return GetCommand()->_flags;
	}

	auto & GetData()
	{
		return _current->_data;
	}
};
}