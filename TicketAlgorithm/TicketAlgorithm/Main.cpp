#include <thread>
#include <iostream>
#include <queue>
#include <mutex>
#include <string>
#include <array>
#include <atomic>
static const int m_processNumbers = 5;
int m_number = 1;
int m_next = 1;
int m_threadNum=0;
std::mutex m_mutex;

std::array<int, m_processNumbers> m_turn;
void cs();
int main()
{
	for (int i = 0; i < m_turn.size(); i++)
	{
		m_turn[i] = 0;
	}

    std::thread t1(cs);
    std::thread t2(cs);

    t1.join();
    t2.join();
	system("pause");
}
void cs()
{
	int i = m_threadNum++;  //Intentional post-increment
	//lock the mutual exclusion
	m_mutex.lock();
	std::cout << "Thread number : " << i << std::endl;
	m_mutex.unlock();
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	while (true)
	{
		//entry protocol
		m_turn[i] = m_number;
		m_number ++;
		m_mutex.lock();
		std::cout << "Thread : " << i << " turn " << m_turn[i] << std::endl;
		m_mutex.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		//skip
		while (m_turn[i] != m_next)
		{
			continue;
		}
		m_mutex.lock();
		std::cout << "Thread : " << i << " critical section " << std::endl;
		m_mutex.unlock();
		//critical section
		m_next ++;
		m_mutex.lock();
		std::cout << "Thread : " << i <<" next " << m_next << std::endl;
		m_mutex.unlock();
	}

}