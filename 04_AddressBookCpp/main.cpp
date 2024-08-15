#include "list.h"
#include "ui.h"

int main() {
	InitList();

	EventLoopRun();

	Commit();
	ReleaseNodeList();
}