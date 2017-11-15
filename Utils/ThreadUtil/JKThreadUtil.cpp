#include "stdafx.h"
#include "JKThreadUtil.h"
#include <thread>
#include <iostream>


void pause_thread(size_t milliseconds)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
	std::cout << "pause of " << milliseconds << " milliseconds ended ----------------thread_id£º" << std::this_thread::get_id << std::endl;
}
