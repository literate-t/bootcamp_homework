#pragma once
#include "DefaultDataContainer.h"
#include "Node.h"
#include <memory>
#include <functional>
#include <vector>
using namespace std;

enum class MenuEnum { QUERY, COMMIT, ROLLBACK, EXIT, END };

struct UserData;
class Menu {
private:
	Menu(DefaultDataContainer* data_container);
	Menu() = default;
	Menu(const Menu&) = delete;
	Menu(Menu&&) = delete;
	Menu& operator=(const Menu&) = delete;
	Menu& operator=(Menu&&) = delete;

public:
	~Menu();

	inline static Menu* GetInstance(DefaultDataContainer* data_container)
	{
		call_once(instance_once_flag_, [&data_container]() {
			instance_ = new Menu(data_container);
			atexit([]() {
				delete instance_;
				});
			});

		return instance_;
	}

	void Run();

	void PrintUser(const UserData* user_data);
	void PrintItem(const UserData* user);
	MenuEnum PrintMenu();
	void ClearBuffer();
	void Pause();

	void InputQuery();
	void InputCommit();
	void InputRollback();

private:
	DefaultDataContainer* data_container_;
	vector <void (Menu::*)()> Inputs;

	static Menu* instance_;
	static once_flag instance_once_flag_;
};
