#include <iostream>
#include <RenderSystem/System.h>
#include <RenderSystem/RenderWindow.h>

void main()
{
	std::cout << "this is main" << std::endl;
	RenderWindowPtr window = GetRS().GetWindow();
	 
}