#include "stdafx.h"
#include <string>
#include <vector>
#include <map>
// 입장 퇴장에 대한 정렬이 필요 없는 경우에는 #include <unordered_map> 사용
#include <unordered_map>
#include <sstream>
#include <iostream>
#include <array>


std::vector<std::string> solution(std::vector<std::string> record) 
{
	std::vector<std::string> answer;
	std::unordered_map<std::string, std::string> idmap;

	for (const auto & it_record : record)
	{
# if 1
		std::istringstream is(it_record);
#else
		std::stringstream ss(it_record);
#endif
		std::string cmd, user_id, user_name;

		is >> cmd;
		if (cmd.compare("Enter") == 0) {
			is >> user_id >> user_name;
			idmap[user_id] = user_name;
		}
		else if(cmd.compare("Leave") == 0) {
			is >> user_id >> user_name;
			idmap[user_id] = user_name;
		}
		else
		{
			break;
		}

	}

	/*
	Prodo 님이 들어왔습니다
	Prodo님이 나갔습니다.
	로그 출력
	*/

	for (const auto & it_record : record) {
		std::istringstream is(it_record);
		std::string cmd, user_id;

		is >> cmd;
		if (cmd.compare("Enter") == 0) {
			is >> user_id;
			answer.push_back(idmap[user_id] + "님이 들어왔습니다");
		}
		else if (cmd.compare("Leave") == 0)
		{
			//cmd.compare("Leave") 일때,,,,,
			is >> user_id;
			answer.push_back(idmap[user_id] + "님이 나갔습니다.");
		}
		else
		{
			break;
		}
	}

	return answer;
}

int main() {

	std::array<std::ostringstream, 6> os;
	std::array<std::string,6> cmd, user_id, user_name;
	std::vector<std::string> set_record;

	cmd[0] = "Enter\n";
	user_id[0] = "kakao\n";
	user_name[0] = "songminkyu\n";

	cmd[1] = "Leave\n";
	user_id[1] = "kakao\n";
	user_name[1] = "songminkyu\n";

	os[0] << cmd[0] << user_id[0] << user_name[0];	
	os[1] << cmd[1] << user_id[1] << user_name[1];
	
	for (auto && it : os)
	{
		set_record.push_back(it.str());
	}
	
			
	for (auto && get_answer : solution(set_record)) {
		std::cout << get_answer << std::endl;
	}
	

	return 0;
}