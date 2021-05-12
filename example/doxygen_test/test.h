/**
 * @file test.h
 * @brief 代码测试文档
 * @author wangpengcheng  (wangpengcheng2018@gmail.com)
 * @version 1.0
 * @date 2021-05-06 12:56:43
 * @copyright Copyright (c) 2021  IRLSCU 
 * 
 * @par 修改日志:
 * <table>
 * <tr>
 *    <th> Commit date</th>
 *    <th> Version </th> 
 *    <th> Author </th>  
 *    <th> Description </th>
 * </tr>
 * <tr>
 *    <td> 2021-05-06 12:56:43 </td>
 *    <td> 1.0 </td>
 *    <td> wangpengcheng </td>
 *    <td>内容</td>
 * </tr>
 * </table>
 */

/**                                   | 文件注释
 * @file apply.c                      | “@file”后的文件名需与当前文件名一致
 * @author clover/clover@123.com
 * @version 1.0
 * @date 2013-12-12
 * @brief 概述：doxygen使用文档
 * 详细介绍了doxygen的C++注释方法
 * @details 详细说明
 * @see MainWindow参考其它的相关的函数，这里作一个链接 url
 * @note 描述需要注意的问题
 */



/// This macro is toolong, so comment here briefly!     | 推荐使用简洁的宏注释
#define HTTP_REQUEST_LEN_MAX    APPLY_BUF_SIZE_BIG
/**
 * The detail macro comment, may be multi-line.  | 尽量少写宏函数，可以使用内联函数代替
 * @param a The brief parameter comment
 * @param b The brief parameter comment
 * @return The brief return value comment
 */
#define MAX(a, b) ((a) > (b))? (a) : (b)


/**
 * @brief 结构体                          | 结构体成员的详细注释写在该成员上面
 * (与名称后面的描述有一个就可以)           | 结构体成员的详细注释与上一成员间留1个空行
 *                                        | 推荐使用简洁的结构体注释
 */
struct StructVariable {    ///  @brief 简单的描述   |  “///”与注释间留有2个空格
    int a;      ///< variable a                     |  “///<”与注释间留有1个空格
    int b;      ///< variable b

    /** this is details mement comment */
    int c;      ///< variable c
    int d;      ///< variable d
};


/** 
 * @enum 性别枚举
 */
enum Sex {   ///  @enum 性别枚举
    kMale,   ///< enum male
    kFemale  ///< enum female
};


/**
 *  @brief 主窗口
 * 对应抽象类
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    /**
     * @brief Construct a new Main Window object
     * @param  parent           QWidget父指针
     */
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    /**
     * @brief 函数名称：setProName                      |尽量避免函数声明和定义重复注释
     * @todo 代码实现的功能: 设置工区名称
     * @param 参数：QWidget*
     * @return 说明：int
     * @retval 1. true 名字设置成功  (返回值说明(可选))
     * @retval 2. false 名字设置失败
     * @bug 此处的bug描述: 无
     */
    bool SetProName(QString name);
 
private:
    QString m_name_;
};


/// @brief 函数名称：setProName
static int ApplyLogin();




