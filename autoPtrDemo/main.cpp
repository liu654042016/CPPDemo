#include <iostream>
#include <memory>


int main(int, char**) {

//**************************************shared_ptr***************************************//
    int a = 10;
    std::shared_ptr<int> ptra = std::make_shared<int>(a); 
    std::shared_ptr<int> ptra2(ptra);//copy
    std::cout<<ptra.use_count()<<std::endl;
    
    int b= 20;
    int* pb=&a;
    //std::shared_ptr<int> ptrb = pb;/*error*/
    std::shared_ptr<int> ptrb = std::make_shared<int>(b);
    ptra2 = ptrb;//assign
    pb = ptrb.get();//获取原始指针

    std::cout<<ptra.use_count()<<std::endl;
    std::cout<<ptrb.use_count()<<std::endl;

//******************************uniqe_ptr*********************************************//
    std::unique_ptr<int> uptr(new int(10));//动态绑定对象

  //  std::unique_ptr<int> uptr2 = uptr; 不能赋值
   // std::unique_ptr<int> upr2(uptr); 不能拷贝
   std::unique_ptr<int> uptr2 = std::move(uptr); //转换所有权
   uptr2.release();//释放所有权
    //************************weak_ptr***********************************//
    

    std::cout<<"hello world3333333333333"<<std::endl;
}
