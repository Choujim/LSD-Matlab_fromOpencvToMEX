#include <mex.h>
#include <lsd.h>
#include <cv.h>
#include <vector>
#include <string>
#include <highgui.h>

#include <cv.hpp>

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
    // 创建全为0的空map
    cv::Mat lsd_map = cv::Mat::zeros(image->ysize, image->xsize, CV_8UC1);
    // 把LSD识别的直线用端点和宽度画出来
    cv::Point pt1, pt2;
    for (int j = 0; j != ntl->size ; ++j)
    {
        // 共5行，按行展开索引，每两行对应列为一个坐标点的(x,y)坐标
        pt1.x = ntl->values[0 + j * ntl->dim];
        pt1.y = ntl->values[1 + j * ntl->dim];
        pt2.x = ntl->values[2 + j * ntl->dim];
        pt2.y = ntl->values[3 + j * ntl->dim];
        double width = ntl->values[4 + j * ntl->dim];
        cv::line(lsd_map, pt1, pt2, cv::Scalar(255), width, CV_AA);
    }

    free_ntuple_list(ntl);

    uchar *mat_array = lsd_map.ptr<uchar>(0);
    // 把lsd_map非0的指针位置标出
    std::vector<int> pos;
    for (int i = 0; i != image->ysize * image->xsize; ++i)
    {
        if (mat_array[i] != 0)
        {
            pos.push_back(i + 1);
        }
    }
    // 创建输出矩阵
    plhs[0] = mxCreateDoubleMatrix(1, pos.size(), mxREAL);
    // 获取输出矩阵的指针
    double *pos_mat = mxGetPr(plhs[0]);
    
    for (int i = 0; i != pos.size(); ++i)
    {
        pos_mat[i] = double(pos[i]);
    }
}