#include <memory>
#include "DefaultDataContainer.h"
#include "Menu.h"
using namespace std;

int main() {
	DefaultDataContainer* data_container = DefaultDataContainer::GetInstance();
	Menu* menu = Menu::GetInstance(data_container);

	menu->Run();
}