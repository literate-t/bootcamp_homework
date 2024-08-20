#include <string>
#include <iostream>
#include <conio.h>
#include <cstdlib>
using namespace std;
#include "Menu.h"

string messages[] = {"나이를 입력하세요: ", "이름을 입력하세요: ", "전화 번호를 입력하세요: "};

Menu* Menu::instance_;
once_flag Menu::instance_once_flag_;

Menu::Menu(DefaultDataContainer* data_container): data_container_(data_container) {
	Inputs.push_back(&Menu::InputQuery);
	Inputs.push_back(&Menu::InputCommit);
	Inputs.push_back(&Menu::InputRollback);
}

Menu::~Menu()
{
	InputCommit();
}

void Menu::Run() {
	MenuEnum menu;

	while ((menu = PrintMenu()) < MenuEnum::END && 0 <= static_cast<int>(menu)) {

		if (MenuEnum::EXIT == menu)
		{
			puts("Bye!");
			return;
		}
		int index = static_cast<int>(menu);
		(this->*Inputs[index])();

		Pause();
	}	
}

void Menu::PrintUser(const UserData* user) {
	printf("%s %s %d세 %s\n", 
		user->name_,
		user->address_,
		user->age_,
		user->phone_);
}

void Menu::PrintItem(const UserData* user) {
	if (nullptr == user) return;

	printf("%s %s %d세 %s\t\n",
		user->name_,
		user->address_,
		user->age_,
		user->phone_);
}

MenuEnum Menu::PrintMenu() {
	int input = static_cast<int>(MenuEnum::END);

	do {
		system("cls");
		puts("[1]Input Query [2]Commit [3]Rollback [4]Exit");
		printf("Input the number: ");
		scanf_s("%d", &input);
		ClearBuffer();
	} while (!(0 < input && input <= static_cast<int>(MenuEnum::END)));


	return static_cast<MenuEnum>(input - 1);
}

void Menu::ClearBuffer() {
	while (getchar() != '\n') {}
}

void Menu::Pause() {
	_getch();
}

void Menu::InputQuery() {
	string query;
	cout << "Query> ";
	getline(cin, query);

	vector<unique_ptr<UserDataManager>> result = data_container_->Query(query);

	if (result.empty()) {
		cout << "<No data>" << endl;
	}
	else {
		cout << "<data list>" << endl;
		for (const auto& item : result) {
			PrintItem(item->GetUserData());
		}
	}
}

void Menu::InputCommit()
{
	puts("Commit!");
	data_container_->Commit();
}

void Menu::InputRollback()
{
	puts("Rollback!");
	data_container_->Clear();
}
