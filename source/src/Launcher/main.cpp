#include <iostream>
#include <RenderSystem/System.h>
#include <RenderSystem/RenderWindow.h>

void main()
{
	std::cout << "this is main" << std::endl;
	GetSystem().Init();
	GetSystem().Run();
	GetSystem().Terminate();

	/*RenderWindowPtr window = GetRS().GetWindow();*/
	 
}