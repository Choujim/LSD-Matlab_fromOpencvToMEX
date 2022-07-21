#include <mex.h>
#include <lsd.h>
#include <cv.h>
#include <vector>
#include <string>
#include <highgui.h>

// nlhs:     等号左边的参数个数
// * plhs[]: 等号左边参数的指针
// nrhs:     等号右边的参数个数
// * prhs[]: 等号右边参数的指针


void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    // std::string picture = mxArrayToString(prhs[0]);
    // cv::Mat src = cv::imread(picture.c_str(), CV_LOAD_IMAGE_COLOR);
    // cv::Mat tmp, src_gray;
    // cv::cvtColor(src, tmp, CV_RGB2GRAY);
    // tmp.convertTo(src_gray, CV_64FC1);

    // 获得输入的图像的 宽w 和 高h
    // 由于matlab中进行了转置以适应LSD中的指针，所以宽高是互换的
    unsigned int h, w;
    w = mxGetM(prhs[0]);
    h = mxGetN(prhs[0]);

    // cv::Mat src_gray(h, w, CV_64FC1);

    // 指向图像第一个元素的double指针
    double* src_gray;

    // 注意mex的prhs默认将矩阵按列展开，对prhs的地址位移需要尤其注意
    src_gray = mxGetPr(prhs[0]);

    // 利用LSD的函数申请 xsize * ysize * double的内存空间给图像
    image_double image = new_image_double(w, h);
    image->data = src_gray;

    // image_double image = new_image_double(src_gray.cols, src_gray.rows);
    // image->data = src_gray.ptr<double>(0);



    ntuple_list ntl = lsd(image);
    // 创建输出矩阵
    plhs[0] = mxCreateDoubleMatrix(5, ntl->size, mxREAL);
    // 获取输出矩阵的指针
    double *pos_mat = mxGetPr(plhs[0]);

    // 写入LSD识别的直线rectangle的端点坐标和宽度到输出矩阵
    for (int j = 0; j != ntl->size ; ++j)
    {
        // plhs共5行，按列展开索引，先x后y
        // +1是由于matlab矩阵索引从1开始，而cv图像矩阵索引从0开始
        pos_mat[5 * j + 0] = ntl->values[0 + j * ntl->dim] + 1;
        pos_mat[5 * j + 2] = ntl->values[1 + j * ntl->dim] + 1;
        pos_mat[5 * j + 1] = ntl->values[2 + j * ntl->dim] + 1;
        pos_mat[5 * j + 3] = ntl->values[3 + j * ntl->dim] + 1;
        pos_mat[5 * j + 4] = ntl->values[4 + j * ntl->dim]; // width
    }
}