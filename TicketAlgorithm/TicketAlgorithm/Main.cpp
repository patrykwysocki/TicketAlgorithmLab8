#include <iostream>
#include <atomic>
#include <thread>
#include <mutex>
#include <sstream>
#include "Vector2D.h"
class Player
{
public:
	Vector2D m_position;
	Player() 
	{

	}
	Player(Vector2D t_position) :
		m_position(t_position)
	{

	}

};

using namespace std;

const int n = 4;
int newTicket;
int currentOrder;
int activeOrders[n] = { 0,0,0,0 };
int numThreads;
mutex m_mutex;
Vector2D m_position = Vector2D(1.0f, 1.0f);
Player m_player(m_position);

void func()
{
	int i = numThreads++;
	
	m_mutex.lock();
	cout << "Current Thread: " << i <<  endl;
	m_mutex.unlock();

	this_thread::sleep_for(chrono::seconds(1));

	while (true)
	{
		activeOrders[i] = newTicket;
		newTicket++;

		m_mutex.lock();
		cout << "Current Thread: " << i << ".\Order number : " << activeOrders[i] << endl;
		cout << endl;
		m_mutex.unlock();

		this_thread::sleep_for(chrono::seconds(1));

		while (activeOrders[i] != currentOrder)
		{
			continue;
		}
		switch (i)
		{
		case 0:
			std::cout << "Render update :" << endl;
			m_player.m_position.x += 2.0f;
			break;
		case 1:
			std::cout << "Input update :" << endl;
			m_player.m_position.y += 2.0f;
			break;
		case 2:
			std::cout << "Physics update : " << endl;
			m_player.m_position.x -= 1.0f;
			break;
		case 3:
			std::cout << "AI update : " << endl;
			m_player.m_position.y -= 1.0f;
			break;
		default:
			m_player.m_position += 10.0f;
			break;
		}
		m_mutex.lock();
		cout << "Player x : " << m_player.m_position.x << "\tPlayer y : " << m_player.m_position.y << endl;
		cout << "thread number: " << i << "\tnow being served Order number : " << activeOrders[i] << endl;
		m_mutex.unlock();

		currentOrder++;;

		m_mutex.lock();
		//cout << "next order is " << currentOrder << endl;
		m_mutex.unlock();
	}
}


int main()
{
	srand(time(NULL));

	numThreads = 0;
	newTicket = 1;
	currentOrder = 1;
	for (int i = 0; i < n; i++)
	{
		activeOrders[i] = 0;
	}

	thread threadRender = thread(func);
	thread threadInput = thread(func);
	thread threadPhysics = thread(func);
	thread threadAI = thread(func);

	threadRender.join();
	threadInput.join();
	threadPhysics.join();
	threadAI.join();

	return 0;
}