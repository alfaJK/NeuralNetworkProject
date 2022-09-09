#include "NeuralController/Conv_neural.h"
#include "IOData/Images.h"
#include "NeuralController/Setting.h"


int main() {
	Neural* CONV_NEURAL_NET = new Neural("config.txt");
	CONV_NEURAL_NET->Start();
	std::cout << "END , WRITE ANY SUMBOL: ";
	char k;
	std::cin >> k;
	
	/*Image* Test;
	Setting* Neural_setting = new Setting();
	Neural_setting->Load_Setting("..\\config.txt");
	std::string road = Neural_setting->path + "1" + "\\" + Neural_setting->name + "1" + " (28).jpg";
	Test = new Image(Neural_setting->SizeImage , road.c_str());
	std::cout << Test->NewImageBits << std::endl;*/

	return 0;
}
