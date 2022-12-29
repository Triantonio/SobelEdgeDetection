#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    /// \brief chooseFile
    void loadImage();

    /// \brief chooseFile
    void sobelImage();

    /// \brief chooseFile
    void sobelOpencvImage();

private:
    /// \brief Refers to the main window interface.
    Ui::MainWindow *mUi;

    /// \brief It connects the interface elements with the different functions found in the project.
    void connectWidgets();

    /// \brief Saves the path where the image to be processed is located.
    std::string mImagePath;

    /// \brief The number of pixels on the vertical axis of the image to be processed.
    int mCols;

    /// \brief The number of pixels on the horizontal axis of the image to be processed.
    int mRows;

    /// \brief The matrix corresponding to the horizontal derivative approximation which results in an approximate
    /// gradient and when combined with that of the vertical axis, the processed image is obtained.
    int mSobelX[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};

    /// \brief The matrix corresponding to the vertical derivative approximation which results in an approximate
    /// gradient and when combined with that of the horizontal axis, the processed image is obtained.
    int mSobelY[3][3] = {{1,2,1},{0,0,0},{-1,-2,-1}};
};
#endif // MAINWINDOW_HPP
