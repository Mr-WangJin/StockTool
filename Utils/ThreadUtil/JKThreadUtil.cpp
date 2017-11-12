#include "stdafx.h"
#include "JKThreadUtil.h"
#include <thread>
#include <iostream>


void pause_thread(int seconds)
{
	std::this_thread::sleep_for(std::chrono::seconds(seconds));
	std::cout << "pause of " << seconds << " seconds ended ----------------thread_id£º" << std::this_thread::get_id << std::endl;
}
