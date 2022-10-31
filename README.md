# TinyRenderer-CN
TinyRenderer是一个从零开始用C++写一个光栅化渲染器，不依赖任何外部库，实现一个超级简化版的OpenGL，本项目作为该项目的翻译

## lesson0-前言
500行代码向你展示OpenGL中的渲染基本工作原理！

本项目是一个微型光栅器，如果您正在寻找关于一个微型光线追踪器的Demo，可以看看原作者的另一个开源项目 [tinyraytracer](https://github.com/ssloy/tinyraytracer)。

其实这份代码具体怎么写的并不重要。阅此读 wiki 并实现您自己的渲染器。只有当你真正体会过这些实现细节时，你才会知道发生到底了什么，自己做一遍是非常重要的。

如果有任何问题，可以通过电子邮件获取反馈，[dmitry.sokolov@univ-lorraine.fr](mailto:dmitry.sokolov@univ-lorraine.fr)。另外如果翻译相关问题，可以直接提issues，或者发邮件[zchanglin@163.com](mailto:zchanglin@163.com)。

如果您是一名教师并想为您的班级使用/改编此项目 - 非常欢迎您这样做，无需授权。只需通过邮件通知我，它将帮助我改进这个教程。

在本系列文章中，我想通过一个非常简化工作流来展示 OpenGL 的工作方式。令人惊讶的是，我经常遇到在最初学习 OpenGL/DirectX 无法克服障碍的人。因此，我准备了一系列简短的讲座，之后我的学生展示了相当不错的渲染器。

所以，任务制定如下：不使用第三方库（尤其是图形库），得到如下图：
![](./docs/image/01.png)
