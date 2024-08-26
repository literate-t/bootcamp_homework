#include "FileManager.h"
#include "UserData.h"
#include "Node.h"

FileManager::FileManager(string file_name, string mode) {
	fopen_s(&fp_, file_name.c_str(), mode.c_str());
	if (nullptr == fp_) {
		puts("file open failure");
	}
}

FileManager::~FileManager() {
	fclose(fp_);
}

size_t FileManager::read(void* data, size_t data_size, size_t element_count)
{
	return fread(data, data_size, element_count, fp_);
}

void FileManager::ProcessDeleteNode(string file_name, Node* node)
{
	UserData* user = node->GetUserData();
	fseek(fp_, node->GetOffset() * sizeof UserData, SEEK_SET);
	UserData init;
	
	if (0 < fwrite(&init, sizeof UserData, 1, fp_))
	{
		printf("delete: %s %s %s\n", user->name_, user->address_, user->phone_);

		FILE* fp_delete = nullptr;
		fopen_s(&fp_delete, file_name.c_str(), "ab+");
		if (nullptr == fp_delete) return;

		int offset = node->GetOffset();
		if (0 < fwrite(&offset, sizeof (int), 1, fp_delete))
		{
			node->MarkDeleteAs(false);
			puts("set deleted offset\n");
		}
		else
		{
			puts("set deleted offset failure\n");
		}

		fclose(fp_delete);		
	}
	else
	{
		puts("set deleted failure\n");
	}
}

void FileManager::ProcessNewNode(string file_name, Node* node)
{
	// find deleted offset
	FILE* fp_deleted = nullptr;
	fopen_s(&fp_deleted, file_name.c_str(), "rb+");
	if (nullptr == fp_deleted)
	{
		printf("fopen_s error: %s\n", file_name.c_str());
		return;
	}
	// set a cursor to start
	fseek(fp_deleted, 0, SEEK_SET);

	int deleted_offset = -1;
	int offset = -1;
	while (0 < fread(&deleted_offset, sizeof(int), 1, fp_deleted))
	{
		++offset;
		if (-1 == deleted_offset) continue;

		// after found deleted_offset
		// and set that offset to file db
		fseek(fp_, (sizeof UserData) * deleted_offset, SEEK_SET);
		node->SetOffset(deleted_offset);

		// set deleted_offset to -1 when deleted_offset is used
		fseek(fp_deleted, offset * sizeof(int), SEEK_SET);
		int complete = -1;
		if (fwrite(&complete, sizeof(int), 1, fp_deleted) <= 0)
		{
			puts("failure: set deleted_offset to -1");
		}

		fclose(fp_deleted);
		break;
	}

	if (-1 == deleted_offset)
	{
		fseek(fp_, 0, SEEK_END);
	}

	UserData* user = node->GetUserData();
	if (0 < fwrite(user, sizeof UserData, 1, fp_))
	{
		node->MarkNewAs(false);
		printf("insert: %s %s %s %d¼¼\n", user->name_, user->address_, user->phone_, user->age_);
	}
	else
	{
		puts("insert failure");
	}

	fclose(fp_deleted);
}

void FileManager::ProcessUpdateNode(string file_name, Node* node)
{
	fseek(fp_, node->GetOffset() * sizeof UserData, SEEK_SET);
	UserData* user = node->GetUserData();
	if (0 < fwrite(user, sizeof UserData, 1, fp_))
	{
		node->MarkUpdateAs(false);
		printf("update: %s %s %s %d¼¼\n", user->name_, user->address_, user->phone_, user->age_);
	}
	else
	{
		puts("update failure");
	}
}
