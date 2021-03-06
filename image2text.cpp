#include"image2text.h"


Image2Text::Image2Text(string input_image_address)
{
	input_image = imread(input_image_address, IMREAD_UNCHANGED);			//打开方式为：IMREAD_UNCHANGED,8位，彩色或非彩色
	if (input_image.empty()) {							//打开错误时发送错误信息
		cerr << "Error! The input image can't be read...\n";
		system("pause");
		exit(0);
	}

}
Image2Text::~Image2Text() {
	delete[] huidu_matrix.matrix;
	delete[] char_image;
	delete[] color_matrix.matrix;
}

void Image2Text::control_output_format(string& outputAddress, int outputFormat)
{
	//根据地址string后几位，判断后几位是否为.txt 或 .html，不是的话，在地址结尾加上
	switch (outputFormat)
	{
	case HTML:
	{
		if (outputAddress.length()<5 || outputAddress.substr(outputAddress.length() - 5, 5) != ".html") {
			outputAddress += ".html";
		}
		break;
	}
	case TXT:
		if (outputAddress.length()<4 || outputAddress.substr(outputAddress.length() - 4, 4) != ".txt") {
			outputAddress += ".txt";
		}
		break;
	default:
	{
		cerr << "The format of the output file only can be .html or .txt\n";
		system("pause");
		exit(0);
		break;
	}
	}
}

Image2Text::huiduMatrix Image2Text::RGB_to_huiduMatrix(Mat RGB, int type, int width, int height) 
{											//返回值为类内声明的结构体
	//彩图 转 灰度图片 
	Mat huidu_image, result_image;
	cvtColor(RGB, huidu_image, COLOR_BGR2GRAY);					//直接调用cvtColor函数，通过第三个参数设置转换方式为彩图转灰度图片

	switch (type)
	{
	case NONDESTRUCTIVE_TXT:
	{
		//输出像素值的变化
		if (width == NONINPUT && huidu_image.cols <= NONDESTRUCTIVE_WIDTH && huidu_image.rows <= NONDESTRUCTIVE_HEIGHT) {
											//当未设置输出图片宽度宽度且图片宽高分别比NONDESTRUCTIVE_WIDTH和NONDESTRUCTIVE_HEIGHT小时，不对图片进行操作
			result_image = huidu_image.clone();
		}
		else {
			int image_width = huidu_image.cols;				//初始值为输入图像宽度，会随图片压缩处理而改变
			int image_height = huidu_image.rows;				//初始值为输入图像高度，会随图片压缩处理而改变
			if (width == NONINPUT || width > NONDESTRUCTIVE_WIDTH || height > NONDESTRUCTIVE_HEIGHT) {
											//当用户没有自定义输出图片的宽度时
				if (image_width >= NONDESTRUCTIVE_WIDTH) {		//宽度判断优先，若图片像素宽度大于NONDESTRUCTIVE_WIDTH，则以宽度为NONDESTRUCTIVE_WIDTH对图片按比例缩小
					height = image_height * (NONDESTRUCTIVE_WIDTH / ((double)image_width));
					image_height = height;
					width = NONDESTRUCTIVE_WIDTH;
					image_width = width;
				}
				if (image_height >= NONDESTRUCTIVE_HEIGHT) {		//判断高度（包括宽度不符合时经处理变化后的高度）是否符合
											//若图片像素高度大于NONDESTRUCTIVE_HEIGHT，则以高度为NONDESTRUCTIVE_HEIGHT对图片按比例进行缩小
					width = image_width * (NONDESTRUCTIVE_HEIGHT / ((double)image_height));
					height = NONDESTRUCTIVE_HEIGHT;
				}
			}
			resize(huidu_image, result_image, Size(width, height));		//对图片进行宽度改为width，高度改为height的变化
		}
		break;
	}
	case NONDESTRUCTIVE_HTML:
	{
		//输出像素值的变化
		if (width == NONINPUT && huidu_image.cols <= NONDESTRUCTIVE_HTML_WIDTH && huidu_image.rows <= NONDESTRUCTIVE_HTML_HEIGHT) {
											//当未设置输出图片宽度宽度且图片宽高分别比NONDESTRUCTIVE_HTML_WIDTH和NONDESTRUCTIVE_HTML_HEIGHT小时，不对图片进行操作
			result_image = huidu_image.clone();
		}
		else {
			int image_width = huidu_image.cols;				//初始值为输入图像宽度，会随图片压缩处理而改变
			int image_height = huidu_image.rows;				//初始值为输入图像高度，会随图片压缩处理而改变
			if (width == NONINPUT || width > NONDESTRUCTIVE_HTML_WIDTH || height > NONDESTRUCTIVE_HTML_HEIGHT) {
											//当用户没有自定义输出图片的宽度时
				if (image_width >= NONDESTRUCTIVE_HTML_WIDTH) {		//宽度判断优先，若图片像素宽度大于NONDESTRUCTIVE_HTML_WIDTH，则以宽度为NONDESTRUCTIVE_HTML_WIDTH对图片按比例缩小
					height = image_height * (NONDESTRUCTIVE_HTML_WIDTH / ((double)image_width));
					image_height = height;
					width = NONDESTRUCTIVE_HTML_WIDTH;
					image_width = width;
				}
				if (image_height >= NONDESTRUCTIVE_HTML_HEIGHT) {	//判断高度（包括宽度不符合时经处理变化后的高度）是否符合
											//若图片像素高度大于NONDESTRUCTIVE_HTML_HEIGHT，则以高度为NONDESTRUCTIVE_HTML_HEIGHT对图片按比例进行缩小
					width = image_width * (NONDESTRUCTIVE_HTML_HEIGHT / ((double)image_height));
					height = NONDESTRUCTIVE_HTML_HEIGHT;
				}
			}
			resize(huidu_image, result_image, Size(width, height));		//对图片进行宽度改为width，高度改为height的变化
		}
		break;
	}
	case NONDESTRUCTIVE_COLOR_HTML:
	{
		//输出像素值的变化
		if (width == NONINPUT && huidu_image.cols <= NONDESTRUCTIVE_COLORHTML_WIDTH && huidu_image.rows <= NONDESTRUCTIVE_COLORHTML_HEIGHT) {
											//当未设置输出图片宽度宽度且图片宽高分别比NONDESTRUCTIVE_COLORHTML_WIDTH和NONDESTRUCTIVE_COLORHTML_HEIGHT小时，不对图片进行操作
			result_image = huidu_image.clone();
		}
		else {
			int image_width = huidu_image.cols;				//初始值为输入图像宽度，会随图片压缩处理而改变
			int image_height = huidu_image.rows;				//初始值为输入图像高度，会随图片压缩处理而改变
			if (width == NONINPUT || width > NONDESTRUCTIVE_COLORHTML_WIDTH || height > NONDESTRUCTIVE_COLORHTML_HEIGHT) {
											//当用户没有自定义输出图片的宽度时
				if (image_width >= NONDESTRUCTIVE_COLORHTML_WIDTH) {	//宽度判断优先，若图片像素宽度大于NONDESTRUCTIVE_COLORHTML_WIDTH，则以宽度为NONDESTRUCTIVE_COLORHTML_WIDTH对图片按比例缩小
					height = image_height * (NONDESTRUCTIVE_COLORHTML_WIDTH / ((double)image_width));
					image_height = height;
					width = NONDESTRUCTIVE_COLORHTML_WIDTH;
					image_width = width;
				}
				if (image_height >= NONDESTRUCTIVE_COLORHTML_HEIGHT) {	//判断高度（包括宽度不符合时经处理变化后的高度）是否符合
											//若图片像素高度大于NONDESTRUCTIVE_COLORHTML_HEIGHT，则以高度为NONDESTRUCTIVE_COLORHTML_HEIGHT对图片按比例进行缩小
					width = image_width * (NONDESTRUCTIVE_COLORHTML_HEIGHT / ((double)image_height));
					height = NONDESTRUCTIVE_COLORHTML_HEIGHT;
				}
			}
			resize(huidu_image, result_image, Size(width, height));		//对图片进行宽度改为width，高度改为height的变化
		}
		break;
	}
	case CHAR_TXT:
	{
		//输出像素值的变化
		if (width == NONINPUT && huidu_image.cols <= CHARIMAGE_WIDTH && huidu_image.rows <= CHARIMAGE_HEIGHT) {
											//当未设置输出图片宽度宽度且图片宽高分别比CHARIMAGE_WIDTH和CHARIMAGE_HEIGHT小时，不对图片进行操作
			result_image = huidu_image.clone();
		}
		else {
			int image_width = huidu_image.cols;				//初始值为输入图像宽度，会随图片压缩处理而改变
			int image_height = huidu_image.rows;				//初始值为输入图像高度，会随图片压缩处理而改变
			if (width == NONINPUT || width >CHARIMAGE_WIDTH || height > CHARIMAGE_HEIGHT) {
											//当用户没有自定义输出图片的宽度时
				if (image_width >= CHARIMAGE_WIDTH) {			//宽度判断优先，若图片像素宽度大于CHARIMAGE_WIDTH，则以宽度为CHARIMAGE_WIDTH对图片按比例缩小
					height = image_height * (CHARIMAGE_WIDTH / ((double)image_width));
					image_height = height;
					width = CHARIMAGE_WIDTH;
					image_width = width;
				}
				if (image_height >= CHARIMAGE_HEIGHT) {			//判断高度（包括宽度不符合时经处理变化后的高度）是否符合
											//若图片像素高度大于CHARIMAGE_HEIGHT，则以高度为CHARIMAGE_HEIGHT对图片按比例进行缩小
					width = image_width * (CHARIMAGE_HEIGHT / ((double)image_height));

					height = CHARIMAGE_HEIGHT;
				}
			}
			resize(huidu_image, result_image, Size(width, height));		//对图片进行宽度改为width，高度改为height的变化
		}
		break;
	}
	case CHAR_HTML:
	{
		//输出像素值的变化
		if (width == NONINPUT && huidu_image.cols <= CHARHTML_WIDTH && huidu_image.rows <= CHARHTML_HEIGHT) {
											//当未设置输出图片宽度宽度且图片宽高分别比CHARHTML_WIDTH和CHARHTML_HEIGHT小时，不对图片进行操作
			result_image = huidu_image.clone();
		}
		else {
			int image_width = huidu_image.cols;				//初始值为输入图像宽度，会随图片压缩处理而改变
			int image_height = huidu_image.rows;				//初始值为输入图像高度，会随图片压缩处理而改变
			if (width == NONINPUT || width >CHARHTML_WIDTH || height > CHARHTML_HEIGHT) {
											//当用户没有自定义输出图片的宽度时
				if (image_width >= CHARHTML_WIDTH) {			//宽度判断优先，若图片像素宽度大于CHARHTML_WIDTH，则以宽度为CHARHTML_WIDTH对图片按比例缩小
					height = image_height * (CHARHTML_WIDTH / ((double)image_width));
					image_height = height;
					width = CHARHTML_WIDTH;
					image_width = width;
				}
				if (image_height >= CHARHTML_HEIGHT) {			//判断高度（包括宽度不符合时经处理变化后的高度）是否符合
											//若图片像素高度大于CHARHTML_HEIGHT，则以高度为CHARHTML_HEIGHT对图片按比例进行缩小
					width = image_width * (CHARHTML_HEIGHT / ((double)image_height));
					height = CHARHTML_HEIGHT;
				}
			}
			resize(huidu_image, result_image, Size(width, height));		//对图片进行宽度改为width，高度改为height的变化
		}
		break;
	}
	case COLOR_HTML:
	{
											//输出像素值的变化
		if (width == NONINPUT && huidu_image.cols <= COLORHTML_WIDTH && huidu_image.rows <= COLORHTML_HEIGHT) {
											//当未设置输出图片宽度宽度且图片宽高分别比COLORHTML_WIDTH和COLORHTML_HEIGHT小时，不对图片进行操作
			result_image = huidu_image.clone();
		}
		else {
			int image_width = huidu_image.cols;				//初始值为输入图像宽度，会随图片压缩处理而改变
			int image_height = huidu_image.rows;				//初始值为输入图像高度，会随图片压缩处理而改变
			if (width == NONINPUT || width >COLORHTML_WIDTH || height > COLORHTML_HEIGHT) {	
											//当用户没有自定义输出图片的宽度时
				if (image_width >= COLORHTML_WIDTH) {			//宽度判断优先，若图片像素宽度大于COLORHTML_WIDTH，则以宽度为COLORHTML_WIDTH对图片按比例缩小
					height = image_height * (COLORHTML_WIDTH / ((double)image_width));
					image_height = height;
					width = COLORHTML_WIDTH;
					image_width = width;
				}
				if (image_height >= COLORHTML_HEIGHT) {			//判断高度（包括宽度不符合时经处理变化后的高度）是否符合
											//若图片像素高度大于COLORHTML_HEIGHT，则以高度为COLORHTML_HEIGHT对图片按比例进行缩小
					width = image_width * (COLORHTML_HEIGHT / ((double)image_height));
					height = COLORHTML_HEIGHT;
				}
			}
			resize(huidu_image, result_image, Size(width, height));		//对图片进行宽度改为width，高度改为height的变化
		}
		break;
	}
	default:
	{
		cerr << "the type of the outputImage only can be NONDESTRUCTIVE_TXT, NONDESTRUCTIVE_HTML, NONDESTRUCTIVE_COLOR_HTML, CHAR_TXT, CHAR_HTML, COLOR_HTML\n";
		system("pause");
		exit(0);
		break;
	}
	}

	//灰度图片 转 灰度矩阵
	int nl = result_image.rows;							//行数  
	int nc = result_image.cols;							//列数
	huidu_matrix.matrix = new int[nl*(nc * result_image.channels()) + 1];		//定义灰度矩阵，用一维数组表示二维数组，便于后边操作
											//矩阵列数 = 像素点列数*通道数（此处为灰度图片只有一个gray通道。彩色图片有RGB三个通道）
	huidu_matrix.width = nc;							//灰度矩阵宽度赋值
	huidu_matrix.height = nl;							//灰度矩阵高度赋值
	for (int j = 0; j<nl; j++) {
		uchar* data = result_image.ptr<uchar>(j);				//data记录图片每行像素的起始地址,ptr<uchar>(j)获得第j行的头指针，<uchar>是图片中的数据类型
		for (int i = 0; i<nc; i++) {						//将该行地址下的所有像素记录到灰度矩阵的对应行中
			huidu_matrix.matrix[j*nc + i] = data[i];
		}
	}
	return huidu_matrix;
}

Image2Text::colorMatrix Image2Text::RGB_to_colorMatrix(Mat RGB, int type, int width, int height)
{
	Mat this_RGB;
	switch (type)
	{
	case COLOR_HTML:
	{
		//输出像素值的变化
		if (width == NONINPUT && RGB.cols <= COLORHTML_WIDTH && RGB.rows <= COLORHTML_HEIGHT) {
											//当未设置输出图片宽度宽度且图片宽高分别比COLORHTML_WIDTH和COLORHTML_HEIGHT小时，不对图片进行操作
			this_RGB = RGB.clone();
		}
		else {
			int image_width = RGB.cols;					//初始值为输入图像宽度，会随图片压缩处理而改变
			int image_height = RGB.rows;					//初始值为输入图像高度，会随图片压缩处理而改变
			if (width == NONINPUT || width > COLORHTML_WIDTH || height > COLORHTML_HEIGHT) {
											//当用户没有自定义输出图片的宽度时
				if (image_width >= COLORHTML_WIDTH) {			//宽度判断优先，若图片像素宽度大于COLORHTML_WIDTH，则以宽度为COLORHTML_WIDTH对图片按比例缩小
					height = image_height * (COLORHTML_WIDTH / ((double)image_width));
					image_height = height;
					width = COLORHTML_WIDTH;
					image_width = width;
				}
				if (image_height >= COLORHTML_HEIGHT) {			//判断高度（包括宽度不符合时经处理变化后的高度）是否符合
											//若图片像素高度大于COLORHTML_HEIGHT，则以高度为COLORHTML_HEIGHT对图片按比例进行缩小
					width = image_width * (COLORHTML_HEIGHT / ((double)image_height));
					height = COLORHTML_HEIGHT;
				}
			}
			resize(RGB, this_RGB, Size(width, height));			//对图片进行宽度改为width，高度改为height的变化
		}
		break;
	}
	case NONDESTRUCTIVE_COLOR_HTML:
	{
		//输出像素值的变化
		if (width == NONINPUT && RGB.cols <= NONDESTRUCTIVE_COLORHTML_WIDTH && RGB.rows <= NONDESTRUCTIVE_COLORHTML_HEIGHT) {
											//当未设置输出图片宽度宽度且图片宽高分别比NONDESTRUCTIVE_COLORHTML_WIDTH和NONDESTRUCTIVE_COLORHTML_HEIGHT小时，不对图片进行操作
			this_RGB = RGB.clone();
		}
		else {
			int image_width = RGB.cols;					//初始值为输入图像宽度，会随图片压缩处理而改变
			int image_height = RGB.rows;					//初始值为输入图像高度，会随图片压缩处理而改变
			if (width == NONINPUT || width > NONDESTRUCTIVE_COLORHTML_WIDTH || height > NONDESTRUCTIVE_COLORHTML_HEIGHT) {
											//当用户没有自定义输出图片的宽度时
				if (image_width >= NONDESTRUCTIVE_COLORHTML_WIDTH) {	//宽度判断优先，若图片像素宽度大于NONDESTRUCTIVE_COLORHTML_WIDTH，则以宽度为NONDESTRUCTIVE_COLORHTML_WIDTH对图片按比例缩小
					height = image_height * (NONDESTRUCTIVE_COLORHTML_WIDTH / ((double)image_width));
					image_height = height;
					width = NONDESTRUCTIVE_COLORHTML_WIDTH;
					image_width = width;
				}
				if (image_height >= NONDESTRUCTIVE_COLORHTML_HEIGHT) {	//判断高度（包括宽度不符合时经处理变化后的高度）是否符合
											//若图片像素高度大于NONDESTRUCTIVE_COLORHTML_HEIGHT，则以高度为NONDESTRUCTIVE_COLORHTML_HEIGHT对图片按比例进行缩小
					width = image_width * (NONDESTRUCTIVE_COLORHTML_HEIGHT / ((double)image_height));
					height = NONDESTRUCTIVE_COLORHTML_HEIGHT;
				}
			}
			resize(RGB, this_RGB, Size(width, height));			//对图片进行宽度改为width，高度改为height的变化
		}
		break;
	}
	default:
	{
		cerr << "the type of colorMatrix only can be COLOR_HTML or NONDESTRUCTIVE_COLOR_HTML\n";
		system("pause");
		exit(0);
		break;
	}
	}
	
	//彩图 转 彩图矩阵
	int nl = this_RGB.rows;
	int nc = this_RGB.cols;
	color_matrix.matrix = new colorPx[nl*(nc * this_RGB.channels()) + 1];
	color_matrix.width = nc;
	color_matrix.height = nl;
	for (int j = 0; j < nl; j++) {
		uchar* color_data = this_RGB.ptr<uchar>(j);
		for (int i = 0; i < nc; i++) {						//图片颜色三通道排列顺序：绿，红，蓝
			color_matrix.matrix[j*nc + i].Green = color_data[3 * i + 1];
			color_matrix.matrix[j*nc + i].Red = color_data[3 * i + 2];
			color_matrix.matrix[j*nc + i].Blue = color_data[3 * i + 3];
		}
	}
	return color_matrix;
}

char* Image2Text::huiduMatrix_to_charTXT(const huiduMatrix &huiduMatrix,int type)
{
	char* huidu_char;
	switch (type) {
	case CHAR_TXT:
	{
		string huidu = "WKQNEFAUBdPTLbqyuzcvri;_:,. ";				//灰度字符
		int huiduStr_length = huidu.length();
		huidu_char = new char [huiduStr_length];
		for (int i = 0; i < huiduStr_length; i++) {
			huidu_char[i] = huidu[i];
		}
		break;
	}
	case REVERSAL_CHAR_TXT:
	{
		string huidu = " .,:,;irvczuyqbLTPdBUAFENQKW";				//反转灰度字符灰度字符
		int huiduStr_length = huidu.length();
		huidu_char = new char[huiduStr_length];
		for (int i = 0; i < huiduStr_length; i++) {
			huidu_char[i] = huidu[i];
		}
		break;
	}
	default:
	{
		cerr << "the type of the charTXT can only be CHAR_TXT and REVERSAL_CHAR_TXT\n";
		system("pause");
		exit(0);
	}
	}
	char_image = new char[huidu_matrix.height*huidu_matrix.width + 1];
	for (int j = 0; j < huidu_matrix.height; j++) {					//根据灰度矩阵的像素值给灰度图片的每个像素匹配字符
		for (int i = 0; i < huidu_matrix.width; i++) {
			if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 8)char_image[j*huidu_matrix.width + i] = huidu_char[0];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 16)char_image[j*huidu_matrix.width + i] = huidu_char[0];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 24)char_image[j*huidu_matrix.width + i] = huidu_char[1];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 32)char_image[j*huidu_matrix.width + i] = huidu_char[2];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 40)char_image[j*huidu_matrix.width + i] = huidu_char[3];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 48)char_image[j*huidu_matrix.width + i] = huidu_char[4];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 56)char_image[j*huidu_matrix.width + i] = huidu_char[5];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 64)char_image[j*huidu_matrix.width + i] = huidu_char[6];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 72)char_image[j*huidu_matrix.width + i] = huidu_char[7];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 80)char_image[j*huidu_matrix.width + i] = huidu_char[8];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 88)char_image[j*huidu_matrix.width + i] = huidu_char[9];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 96)char_image[j*huidu_matrix.width + i] = huidu_char[10];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 104)char_image[j*huidu_matrix.width + i] = huidu_char[11];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 112)char_image[j*huidu_matrix.width + i] = huidu_char[12];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 120)char_image[j*huidu_matrix.width + i] = huidu_char[13];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 128)char_image[j*huidu_matrix.width + i] = huidu_char[14];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 136)char_image[j*huidu_matrix.width + i] = huidu_char[15];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 144)char_image[j*huidu_matrix.width + i] = huidu_char[16];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 152)char_image[j*huidu_matrix.width + i] = huidu_char[17];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 160)char_image[j*huidu_matrix.width + i] = huidu_char[18];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 168)char_image[j*huidu_matrix.width + i] = huidu_char[19];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 176)char_image[j*huidu_matrix.width + i] = huidu_char[20];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 184)char_image[j*huidu_matrix.width + i] = huidu_char[20];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 192)char_image[j*huidu_matrix.width + i] = huidu_char[21];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 200)char_image[j*huidu_matrix.width + i] = huidu_char[22];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 208)char_image[j*huidu_matrix.width + i] = huidu_char[23];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 216)char_image[j*huidu_matrix.width + i] = huidu_char[24];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 224)char_image[j*huidu_matrix.width + i] = huidu_char[25];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 232)char_image[j*huidu_matrix.width + i] = huidu_char[26];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 240)char_image[j*huidu_matrix.width + i] = huidu_char[26];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 248)char_image[j*huidu_matrix.width + i] = huidu_char[27];
			else char_image[j*huidu_matrix.width + i] = huidu_char[27];
		}
	}
	delete[]huidu_char;
	return char_image;
}

char* Image2Text::huiduMatrix_to_charHtml(const huiduMatrix &huiduMatrix,int type)
{
	char* huidu_char;
	switch (type) {
	case CHAR_HTML:
	{
		string huidu = "WKQNEFAUBdPTLbqyuzcvri;_:^,.";				//灰度字符
		int huiduStr_length = huidu.length();
		huidu_char = new char[huiduStr_length];
		for (int i = 0; i < huiduStr_length; i++) {
			huidu_char[i] = huidu[i];
		}
		break;
	}
	case REVERSAL_CHAR_HTML:
	{
		string huidu = ".,:^,;irvczuyqbLTPdBUAFENQKW";				//反转灰度字符灰度字符
		int huiduStr_length = huidu.length();
		huidu_char = new char[huiduStr_length];
		for (int i = 0; i < huiduStr_length; i++) {
			huidu_char[i] = huidu[i];
		}
		break;
	}
	default:
	{
		cerr << "the type of the charHTML can only be CHAR_HTML and REVERSAL_CHAR_HTML\n";
		system("pause");
		exit(0);
	}
	}
	char_image = new char[huidu_matrix.height*huidu_matrix.width + 1];
	for (int j = 0; j < huidu_matrix.height; j++) {					//根据灰度矩阵的像素值给灰度图片的每个像素匹配字符
		for (int i = 0; i < huidu_matrix.width; i++) {
			if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 8)char_image[j*huidu_matrix.width + i] = huidu_char[0];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 16)char_image[j*huidu_matrix.width + i] = huidu_char[0];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 24)char_image[j*huidu_matrix.width + i] = huidu_char[1];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 32)char_image[j*huidu_matrix.width + i] = huidu_char[2];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 40)char_image[j*huidu_matrix.width + i] = huidu_char[3];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 48)char_image[j*huidu_matrix.width + i] = huidu_char[4];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 56)char_image[j*huidu_matrix.width + i] = huidu_char[5];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 64)char_image[j*huidu_matrix.width + i] = huidu_char[6];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 72)char_image[j*huidu_matrix.width + i] = huidu_char[7];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 80)char_image[j*huidu_matrix.width + i] = huidu_char[8];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 88)char_image[j*huidu_matrix.width + i] = huidu_char[9];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 96)char_image[j*huidu_matrix.width + i] = huidu_char[10];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 104)char_image[j*huidu_matrix.width + i] = huidu_char[11];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 112)char_image[j*huidu_matrix.width + i] = huidu_char[12];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 120)char_image[j*huidu_matrix.width + i] = huidu_char[13];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 128)char_image[j*huidu_matrix.width + i] = huidu_char[14];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 136)char_image[j*huidu_matrix.width + i] = huidu_char[15];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 144)char_image[j*huidu_matrix.width + i] = huidu_char[16];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 152)char_image[j*huidu_matrix.width + i] = huidu_char[17];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 160)char_image[j*huidu_matrix.width + i] = huidu_char[18];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 168)char_image[j*huidu_matrix.width + i] = huidu_char[19];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 176)char_image[j*huidu_matrix.width + i] = huidu_char[20];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 184)char_image[j*huidu_matrix.width + i] = huidu_char[20];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 192)char_image[j*huidu_matrix.width + i] = huidu_char[21];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 200)char_image[j*huidu_matrix.width + i] = huidu_char[22];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 208)char_image[j*huidu_matrix.width + i] = huidu_char[23];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 216)char_image[j*huidu_matrix.width + i] = huidu_char[24];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 224)char_image[j*huidu_matrix.width + i] = huidu_char[25];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 232)char_image[j*huidu_matrix.width + i] = huidu_char[26];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 240)char_image[j*huidu_matrix.width + i] = huidu_char[26];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 248)char_image[j*huidu_matrix.width + i] = huidu_char[27];
			else char_image[j*huidu_matrix.width + i] = huidu_char[27];
		}
	}
	delete[]huidu_char;
	return char_image;
}

char* Image2Text::huiduMatrix_to_colorCharHtml(const huiduMatrix &huiduMatrix)
{
	char huidu_char[] = "WKQNEFAUBdPTLbqyuzcvri;_:^,.";				//灰度字符
	char_image = new char[huidu_matrix.height*huidu_matrix.width + 1];
	for (int j = 0; j < huidu_matrix.height; j++) {					//根据灰度矩阵的像素值给灰度图片的每个像素匹配字符
		for (int i = 0; i < huidu_matrix.width; i++) {
			if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 8)char_image[j*huidu_matrix.width + i] = huidu_char[0];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 16)char_image[j*huidu_matrix.width + i] = huidu_char[0];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 24)char_image[j*huidu_matrix.width + i] = huidu_char[1];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 32)char_image[j*huidu_matrix.width + i] = huidu_char[2];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 40)char_image[j*huidu_matrix.width + i] = huidu_char[3];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 48)char_image[j*huidu_matrix.width + i] = huidu_char[4];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 56)char_image[j*huidu_matrix.width + i] = huidu_char[5];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 64)char_image[j*huidu_matrix.width + i] = huidu_char[6];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 72)char_image[j*huidu_matrix.width + i] = huidu_char[7];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 80)char_image[j*huidu_matrix.width + i] = huidu_char[8];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 88)char_image[j*huidu_matrix.width + i] = huidu_char[9];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 96)char_image[j*huidu_matrix.width + i] = huidu_char[10];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 104)char_image[j*huidu_matrix.width + i] = huidu_char[11];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 112)char_image[j*huidu_matrix.width + i] = huidu_char[12];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 120)char_image[j*huidu_matrix.width + i] = huidu_char[13];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 128)char_image[j*huidu_matrix.width + i] = huidu_char[14];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 136)char_image[j*huidu_matrix.width + i] = huidu_char[15];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 144)char_image[j*huidu_matrix.width + i] = huidu_char[16];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 152)char_image[j*huidu_matrix.width + i] = huidu_char[17];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 160)char_image[j*huidu_matrix.width + i] = huidu_char[18];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 168)char_image[j*huidu_matrix.width + i] = huidu_char[19];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 176)char_image[j*huidu_matrix.width + i] = huidu_char[20];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 184)char_image[j*huidu_matrix.width + i] = huidu_char[20];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 192)char_image[j*huidu_matrix.width + i] = huidu_char[21];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 200)char_image[j*huidu_matrix.width + i] = huidu_char[22];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 208)char_image[j*huidu_matrix.width + i] = huidu_char[23];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 216)char_image[j*huidu_matrix.width + i] = huidu_char[24];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 224)char_image[j*huidu_matrix.width + i] = huidu_char[25];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 232)char_image[j*huidu_matrix.width + i] = huidu_char[26];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 240)char_image[j*huidu_matrix.width + i] = huidu_char[26];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 248)char_image[j*huidu_matrix.width + i] = huidu_char[27];
			else char_image[j*huidu_matrix.width + i] = huidu_char[27];
		}
	}
	return char_image;
}

char* Image2Text::huiduMatrix_to_nondestructiveTXT(const huiduMatrix &huidu_matrix,int type) 
{
	char* huidu_char;
	switch (type) {
	case NONDESTRUCTIVE_TXT:
	{
		string huidu = "@#xo+. ";						//灰度字符
		int huiduStr_length = huidu.length();
		huidu_char = new char[huiduStr_length];
		for (int i = 0; i < huiduStr_length; i++) {
			huidu_char[i] = huidu[i];
		}
		break;
	}
	case REVERSAL_NONDESTRUCTIVE_TXT:
	{
		string huidu = " .+ox#@";						//反转灰度字符灰度字符
		int huiduStr_length = huidu.length();
		huidu_char = new char[huiduStr_length];
		for (int i = 0; i < huiduStr_length; i++) {
			huidu_char[i] = huidu[i];
		}
		break;
	}
	default:
	{
		cerr << "the type of the charTXT can only be NONDESTRUCTIVE_TXT and REVERSAL_NONDESTRUCTIVE_TXT\n";
		system("pause");
		exit(0);
	}
	}
	char_image = new char[huidu_matrix.height*huidu_matrix.width + 1];
	for (int j = 0; j < huidu_matrix.height; j++) {					//根据灰度矩阵的像素值给灰度图片的每个像素匹配字符
		for (int i = 0; i < huidu_matrix.width; i++) {				//0-35:@   36-71:#   72-108:x   109-144:o   145-180:+   181-216:-   217-255: 
			if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 35)char_image[j*huidu_matrix.width + i] = huidu_char[0];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 71)char_image[j*huidu_matrix.width + i] = huidu_char[1];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 108)char_image[j*huidu_matrix.width + i] = huidu_char[2];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 144)char_image[j*huidu_matrix.width + i] = huidu_char[3];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 180)char_image[j*huidu_matrix.width + i] = huidu_char[4];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 216)char_image[j*huidu_matrix.width + i] = huidu_char[5];
			else char_image[j*huidu_matrix.width + i] = huidu_char[6];
		}
	}
	delete[]huidu_char;
	return char_image;
}

char* Image2Text::huiduMatrix_to_nondestructiveHtml(const huiduMatrix &huiduMatrix, int type)
{
	char* huidu_char;
	switch (type) {
	case NONDESTRUCTIVE_HTML:
	{
		string huidu = "WKQNEFAUBdPTLbqyuzcvri;_:^,.";				//灰度字符
		int huiduStr_length = huidu.length();
		huidu_char = new char[huiduStr_length];
		for (int i = 0; i < huiduStr_length; i++) {
			huidu_char[i] = huidu[i];
		}
		break;
	}
	case REVERSAL_NONDESTRUCTIVE_HTML:
	{
		string huidu = ".,:^,;irvczuyqbLTPdBUAFENQKW";				//反转灰度字符灰度字符
		int huiduStr_length = huidu.length();
		huidu_char = new char[huiduStr_length];
		for (int i = 0; i < huiduStr_length; i++) {
			huidu_char[i] = huidu[i];
		}
		break;
	}
	default:
	{
		cerr << "the type of the nondestructiveHtml can only be NONDESTRUCTIVE_HTML and REVERSAL_NONDESTRUCTIVE_HTML\n";
		system("pause");
		exit(0);
	}
	}
	char_image = new char[huidu_matrix.height*huidu_matrix.width + 1];
	for (int j = 0; j < huidu_matrix.height; j++) {					//根据灰度矩阵的像素值给灰度图片的每个像素匹配字符
		for (int i = 0; i < huidu_matrix.width; i++) {
			if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 8)char_image[j*huidu_matrix.width + i] = huidu_char[0];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 16)char_image[j*huidu_matrix.width + i] = huidu_char[0];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 24)char_image[j*huidu_matrix.width + i] = huidu_char[1];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 32)char_image[j*huidu_matrix.width + i] = huidu_char[2];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 40)char_image[j*huidu_matrix.width + i] = huidu_char[3];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 48)char_image[j*huidu_matrix.width + i] = huidu_char[4];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 56)char_image[j*huidu_matrix.width + i] = huidu_char[5];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 64)char_image[j*huidu_matrix.width + i] = huidu_char[6];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 72)char_image[j*huidu_matrix.width + i] = huidu_char[7];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 80)char_image[j*huidu_matrix.width + i] = huidu_char[8];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 88)char_image[j*huidu_matrix.width + i] = huidu_char[9];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 96)char_image[j*huidu_matrix.width + i] = huidu_char[10];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 104)char_image[j*huidu_matrix.width + i] = huidu_char[11];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 112)char_image[j*huidu_matrix.width + i] = huidu_char[12];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 120)char_image[j*huidu_matrix.width + i] = huidu_char[13];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 128)char_image[j*huidu_matrix.width + i] = huidu_char[14];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 136)char_image[j*huidu_matrix.width + i] = huidu_char[15];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 144)char_image[j*huidu_matrix.width + i] = huidu_char[16];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 152)char_image[j*huidu_matrix.width + i] = huidu_char[17];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 160)char_image[j*huidu_matrix.width + i] = huidu_char[18];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 168)char_image[j*huidu_matrix.width + i] = huidu_char[19];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 176)char_image[j*huidu_matrix.width + i] = huidu_char[20];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 184)char_image[j*huidu_matrix.width + i] = huidu_char[20];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 192)char_image[j*huidu_matrix.width + i] = huidu_char[21];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 200)char_image[j*huidu_matrix.width + i] = huidu_char[22];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 208)char_image[j*huidu_matrix.width + i] = huidu_char[23];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 216)char_image[j*huidu_matrix.width + i] = huidu_char[24];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 224)char_image[j*huidu_matrix.width + i] = huidu_char[25];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 232)char_image[j*huidu_matrix.width + i] = huidu_char[26];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 240)char_image[j*huidu_matrix.width + i] = huidu_char[26];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 248)char_image[j*huidu_matrix.width + i] = huidu_char[27];
			else char_image[j*huidu_matrix.width + i] = huidu_char[27];
		}
	}
	delete[]huidu_char;
	return char_image;
}


void Image2Text::to_txt(string output_address, int type_of_outputImage, int width, int height) {
	char * result_image;								//记录生成的字符画矩阵
	int nl, nc;									//记录生成的字符画矩阵的宽度nc，高度nl
	switch (type_of_outputImage) {							//根据用户选择的输出图片类型：无损图片或者字符画图片来输出txt
	case NONDESTRUCTIVE_TXT:							//txt格式：宋体 常规体 大小1  max500*500
	{
		result_image = huiduMatrix_to_nondestructiveTXT(RGB_to_huiduMatrix(this->input_image, NONDESTRUCTIVE_TXT, width, height));
		nl = RGB_to_huiduMatrix(this->input_image, NONDESTRUCTIVE_TXT, width, height).height;
		nc = RGB_to_huiduMatrix(this->input_image, NONDESTRUCTIVE_TXT, width, height).width;
		cout << "TXT的最佳文字格式为：宋体 常规 ，大小为 1  ！！";
		system("pause");
		break;
	}
	case CHAR_TXT:									//txt格式：宋体 黑体 大小8  max70*70
	{
		result_image = huiduMatrix_to_charTXT(RGB_to_huiduMatrix(this->input_image, CHAR_TXT, width, height));
		nl = RGB_to_huiduMatrix(this->input_image, CHAR_TXT, width, height).height;
		nc = RGB_to_huiduMatrix(this->input_image, CHAR_TXT, width, height).width;
		cout << "TXT的最佳文字格式为：楷体 常规 ，大小为 10  ！！";
		system("pause");
		break;
	}
	case REVERSAL_CHAR_TXT:
	{
		result_image = huiduMatrix_to_charTXT(RGB_to_huiduMatrix(this->input_image, CHAR_TXT, width, height),REVERSAL_CHAR_TXT);
		nl = RGB_to_huiduMatrix(this->input_image, CHAR_TXT, width, height).height;
		nc = RGB_to_huiduMatrix(this->input_image, CHAR_TXT, width, height).width;
		cout << "TXT的最佳文字格式为：楷体 常规 ，大小为 10  ！！";
		system("pause");
		break;
	}
	case REVERSAL_NONDESTRUCTIVE_TXT:
	{
		result_image = huiduMatrix_to_nondestructiveTXT(RGB_to_huiduMatrix(this->input_image, NONDESTRUCTIVE_TXT, width, height), REVERSAL_NONDESTRUCTIVE_TXT);
		nl = RGB_to_huiduMatrix(this->input_image, NONDESTRUCTIVE_TXT, width, height).height;
		nc = RGB_to_huiduMatrix(this->input_image, NONDESTRUCTIVE_TXT, width, height).width;
		cout << "TXT的最佳文字格式为：宋体 常规 ，大小为 1  ！！";
		system("pause");
		break;
	}
	default:									//type_of_outputImage为未定义的格式时，打印错误信息
	{
		cerr << "the type of the outputImage only can be NONDESTRUCTIVE_TXT, CHAR_TXT, REVERSAL_CHAR_TXT, REVERSAL_NONDESTRUCTIVE_TXT\n";
		system("pause");
		exit(0);
		break;
	}
	}

	//标准化输出文件的命名格式为 xxx.txt
	control_output_format(output_address, TXT);

	//将生成的字符画图片储存
	ofstream f(output_address, ios::out);
	for (int j = 0; j < nl; j++) {
		for (int i = 0; i < nc; i++) {
			f << char_image[j*nc + i];
			f << char_image[j*nc + i];
		}
		f << '\n';
	}
	f.close();
}

void Image2Text::to_html(string output_address, int type_of_output, int width, int height)
{
	switch (type_of_output)
	{
	case CHAR_HTML:
	{
		char * result_image;								//记录生成的字符画矩阵
		int nl, nc;									//记录生成的字符画矩阵的宽度nc，高度nl
		result_image = huiduMatrix_to_charHtml(RGB_to_huiduMatrix(this->input_image, CHAR_HTML, width, height));
		nl = RGB_to_huiduMatrix(this->input_image, CHAR_HTML, width, height).height;
		nc = RGB_to_huiduMatrix(this->input_image, CHAR_HTML, width, height).width;

		//标准化输出文件的命名格式为 xxx.html
		control_output_format(output_address, HTML);

		//.html文件生成保存
		ofstream f(output_address, ios::out);
		f << "<html>\n";
		f << "<head>\n";
		f << "<title>字符画<\/title>\n";
		f << "<\/head>\n";
		f << "<body>\n";
		f << "<div style=\"font-size:10px;font-family:宋体;line-height:0px\">\n";
		for (int j = 0; j < nl; j++)
		{
			f << "<p>";
			for (int i = 0; i < nc; i++)
			{
				f << char_image[j*nc + i];
				f << char_image[j*nc + i];
			}
			f << "<\/p>";
			f << '\n';
		}
		f << "<\/div>";
		f << "<\/body>";
		f << "<\/html>";
		f.close();
		break;
	}
	case REVERSAL_CHAR_HTML:
	{
		char * result_image;								//记录生成的字符画矩阵
		int nl, nc;									//记录生成的字符画矩阵的宽度nc，高度nl
		result_image = huiduMatrix_to_charHtml(RGB_to_huiduMatrix(this->input_image, CHAR_HTML, width, height),REVERSAL_CHAR_HTML);
		nl = RGB_to_huiduMatrix(this->input_image, CHAR_HTML, width, height).height;
		nc = RGB_to_huiduMatrix(this->input_image, CHAR_HTML, width, height).width;

		//标准化输出文件的命名格式为 xxx.html
		control_output_format(output_address, HTML);

		//.html文件生成保存
		ofstream f(output_address, ios::out);
		f << "<html>\n";
		f << "<head>\n";
		f << "<title>字符画<\/title>\n";
		f << "<\/head>\n";
		f << "<body>\n";
		f << "<div style=\"font-size:10px;font-family:宋体;line-height:0px\">\n";
		for (int j = 0; j < nl; j++)
		{
			f << "<p>";
			for (int i = 0; i < nc; i++)
			{
				f << char_image[j*nc + i];
				f << char_image[j*nc + i];
			}
			f << "<\/p>";
			f << '\n';
		}
		f << "<\/div>";
		f << "<\/body>";
		f << "<\/html>";
		f.close();
		break;
	}
	case NONDESTRUCTIVE_HTML:
	{
		char * result_image;								//记录生成的字符画矩阵
		int nl, nc;									//记录生成的字符画矩阵的宽度nc，高度nl
		result_image = huiduMatrix_to_nondestructiveHtml(RGB_to_huiduMatrix(this->input_image, NONDESTRUCTIVE_HTML, width, height));
		nl = RGB_to_huiduMatrix(this->input_image, NONDESTRUCTIVE_HTML, width, height).height;
		nc = RGB_to_huiduMatrix(this->input_image, NONDESTRUCTIVE_HTML, width, height).width;

		//标准化输出文件的命名格式为 xxx.html
		control_output_format(output_address, HTML);

		//.html文件生成保存
		ofstream f(output_address, ios::out);
		f << "<html>\n";
		f << "<head>\n";
		f << "<title>字符画<\/title>\n";
		f << "<\/head>\n";
		f << "<body>\n";
		f << "<div style=\"font-size:1px;font-family:宋体;line-height:0px\">\n";
		for (int j = 0; j < nl; j++)
		{
			f << "<p>";
			for (int i = 0; i < nc; i++)
			{
				f << char_image[j*nc + i];
				f << char_image[j*nc + i];
			}
			f << "<\/p>";
			f << '\n';
		}
		f << "<\/div>";
		f << "<\/body>";
		f << "<\/html>";
		f.close();
		break;
	}
	case REVERSAL_NONDESTRUCTIVE_HTML:
	{
		char * result_image;								//记录生成的字符画矩阵
		int nl, nc;									//记录生成的字符画矩阵的宽度nc，高度nl
		result_image = huiduMatrix_to_nondestructiveHtml(RGB_to_huiduMatrix(this->input_image, NONDESTRUCTIVE_HTML, width, height),REVERSAL_NONDESTRUCTIVE_HTML);
		nl = RGB_to_huiduMatrix(this->input_image, NONDESTRUCTIVE_HTML, width, height).height;
		nc = RGB_to_huiduMatrix(this->input_image, NONDESTRUCTIVE_HTML, width, height).width;

		//标准化输出文件的命名格式为 xxx.html
		control_output_format(output_address, HTML);

		//.html文件生成保存
		ofstream f(output_address, ios::out);
		f << "<html>\n";
		f << "<head>\n";
		f << "<title>字符画<\/title>\n";
		f << "<\/head>\n";
		f << "<body>\n";
		f << "<div style=\"font-size:1px;font-family:宋体;line-height:0px\">\n";
		for (int j = 0; j < nl; j++)
		{
			f << "<p>";
			for (int i = 0; i < nc; i++)
			{
				f << char_image[j*nc + i];
				f << char_image[j*nc + i];
			}
			f << "<\/p>";
			f << '\n';
		}
		f << "<\/div>";
		f << "<\/body>";
		f << "<\/html>";
		f.close();
		break;
	}
	case COLOR_HTML:
	{
		colorMatrix result_matrix = RGB_to_colorMatrix(this->input_image, COLOR_HTML, width, height);
		char * result_image = huiduMatrix_to_colorCharHtml(RGB_to_huiduMatrix(this->input_image, COLOR_HTML, width, height));
		int nl, nc;
		nl = result_matrix.height;
		nc = result_matrix.width;

		//标准化输出文件的命名格式为 xxx.html
		control_output_format(output_address, HTML);

		//.html文件生成保存
		ofstream f(output_address, ios::out);
		f << "<html>\n";
		f << "<head>\n";
		f << "<title>字符画<\/title>\n";
		f << "<\/head>\n";
		f << "<body>\n";
		f << "<div style=\"font-size:10px;font-family:宋体;line-height:0px\">\n";
		for (int j = 0; j < nl; j++)
		{
			f << "<p>";
			for (int i = 0; i < nc; i++)
			{
				f << "<a style=\"color:rgb(" << result_matrix.matrix[j*nc + i].Red << "," << result_matrix.matrix[j*nc + i].Green << "," << result_matrix.matrix[j*nc + i].Blue << ")\">";
				f << result_image[j*nc + i];
				f << result_image[j*nc + i];
				f << "<\/a>";
			}
			f << "<\/p>";
			f << '\n';
		}
		f << "<\/div>";
		f << "<\/body>";
		f << "<\/html>";
		f.close();
		break;
	}
	case NONDESTRUCTIVE_COLOR_HTML:
	{
		colorMatrix result_matrix = RGB_to_colorMatrix(this->input_image, NONDESTRUCTIVE_COLOR_HTML, width, height);
		char * result_image = huiduMatrix_to_colorCharHtml(RGB_to_huiduMatrix(this->input_image, NONDESTRUCTIVE_COLOR_HTML, width, height));
		int nl, nc;
		nl = result_matrix.height;
		nc = result_matrix.width;

		//标准化输出文件的命名格式为 xxx.html
		control_output_format(output_address, HTML);

		//.html文件生成保存
		ofstream f(output_address, ios::out);
		f << "<html>\n";
		f << "<head>\n";
		f << "<title>字符画<\/title>\n";
		f << "<\/head>\n";
		f << "<body>\n";
		f << "<div style=\"font-size:1px;font-family:宋体;line-height:0px\">\n";
		for (int j = 0; j < nl; j++)
		{
			f << "<p>";
			for (int i = 0; i < nc; i++)
			{
				f << "<a style=\"color:rgb(" << result_matrix.matrix[j*nc + i].Red << "," << result_matrix.matrix[j*nc + i].Green << "," << result_matrix.matrix[j*nc + i].Blue << ")\">";
				f << result_image[j*nc + i];
				f << result_image[j*nc + i];
				f << "<\/a>";
			}
			f << "<\/p>";
			f << '\n';
		}
		f << "<\/div>";
		f << "<\/body>";
		f << "<\/html>";
		f.close();
		cout << "HTML文件打开后以浏览器以400%的比例查看最佳噢！！\n";
		system("pause");
		break;
	}
	default:
	{
		cerr << "The type of output only can be CHAR_HTML, COLOR_HTML, REVERSAL_HTML, NONDESTRUCTIVE_HTML, REVERSAL_NONDESTRUCTIVE_HTML, NONDESTRUCTIVE_COLOR_HTML\n";
		system("pause");
		exit(0);
	}
	}
};

