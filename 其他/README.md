## 图像格式
### PNG
* 数据格式

* 获取png元数据

### JPEG
* 数据格式

* 获取jpeg元数据

## opengl
* mesa3d

### opengl api
* https://www.khronos.org/opengl/wiki/Category:Core_API_Reference

* 缓冲区对象
    
        void glGenBuffers(GLsizei n, GLuint *buffers)    // 在buffers数组中返回当前n个未使用的名称，表示缓冲区对象

        void glBindBuffer(GLenum target, GLuint buffer)    // 指定当前活动缓冲区的对象

        void glBufferData(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage)    // target:可以是GL_ARRAY_BUFFER()（顶点数据）或GL_ELEMENT_ARRAY_BUFFER(索引数据)，size:存储相关数据所需的内存容量，data:用于初始化缓冲区对象，可以是一个指向客户区内存的指针，也可以是NULL,usage:数据在分配之后如何进行读写,如GL_STREAM_READ，GL_STREAM_DRAW，GL_STREAM_COPY

* 纹理对象

        void glGenTextures (GLsizei n, GLuint *textures)        // 在数组textures中返回n个当期未使用的值，表示纹理对象的名称，零作为一个保留的纹理对象名，它不会被此函数当做纹理对象名称而返回

        void glBindTexture (GLenum target, GLuint texture)       // 应用程序第一次在这个函数使用texture值时，这个函数将会创建一个新的纹理对象，当texture是一个以前已经创建的纹理对象时，这个纹理对象就成为活动对象，如果texture为0，就停止使用这个纹理对象，并返回无名称的默认纹理

        void glDeleteTextures (GLsizei n, const GLuint *textures)    // 删除n个纹理对象，它们的名称由textures数组提供

        void glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST)       //指定放大和缩小的过滤方法


        void glTexImage2D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels)     //target参数用于定义二维纹理；如果提供了多种分辨率的纹理图像，可以使用level参数，否则level设置为0；internalformat确定了哪些成分(RGBA, 深度, 亮度和强度)被选定为图像纹理单元,width和height表示纹理图像的宽度和高度；border参数表示边框的宽度;format和type参数描述了纹理图像数据的格式和数据类型;pixels参数包含了纹理图像的数据，这个数据描述了纹理图像本身和它的边框


        glClearColor (0.0, 0.0, 0.0, 0.0)   //设置清除颜色,参数分别表示RGBA，取值范围0-1,全0表示白色，全1表示黑色
        glClear(GL_COLOR_BUFFER_BIT)    //把窗口清除为当前颜色
        glClearDepth(1.0)   //指定深度缓冲区中每个像素需要的值
        glClear(GL_DEPTH_BUFFER_BIT)    //清除深度缓冲区


        glShadeModel    // 设置着色模型,平滑还是过度

        glEnable (GL_LINE_STIPPLE | GL_BLEND | GL_DEPTH_TEST | GL_FOG | GL_LIGHTING)    //打开模式
        glDisable   // 禁用模式
        GL_LINE_STIPPLE 直线的点画模式
        GL_BLEND 控制RGBA颜色的混合
        GL_DEPTH_TEST 深度比较
        GL_FOG 雾
        GL_LIGHTING 光照


* 顶点数组
        
        glEnableClientState(GL_VERTEX_ARRAY)   //启用数组
        glVertexPointer(2, GL_INT, 0, vertices) //2表示顶点的坐标数量，GL_INT表示每个坐标数据类型，0表示数组中的顶点是紧密相连的,vertices表示第一个顶点的第一个坐标的内存地址     

        glPixelStorei(GL_UNPACK_ALIGNMENT,1)控制的是所读取数据的对齐方式，默认4字节对齐，即一行的图像数据字节数必须是4的整数倍，即读取数据时，读取4个字节用来渲染一行，之后读取4字节数据用来渲染第二行。对RGB 3字节像素而言，若一行10个像素，即30个字节，在4字节对齐模式下，OpenGL会读取32个字节的数据，若不加注意，会导致glTextImage中致函数的读取越界，从而全面崩溃。



        glMatrixMode()命令将当前矩阵设置成参数所指定的模式，以满足不同绘图所需执行的矩阵变换。一般而言，在需要绘制出对象或要对所绘制对象进行几何变换时，需要将变换矩阵设置成模型视图模式；而当需要对绘制的对象设置某种投影方式时，则需要将变换矩阵设置成投影模式；只有在进行纹理映射时，才需要将变换矩阵设置成纹理模式。

        glLoadIdentity()功能是重置当前指定的矩阵为单位矩阵

* 其他
        
        void APIENTRY gluPerspective (GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);//fovy:角度 aspect：纵横比 zNear：观测点与近侧裁剪截面的距离 zFar：与远侧裁剪截面的距离

        gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0)  // 投影变换的最终目的是定义一个视景体，视景体有两个用途，首先，视景体是决定一个物体是如何映射到屏幕的，其次，视景体定义了哪些物体（或物体的一部分）被裁剪到最终的图像之外

        void gluLookAt (GLdouble eyex, GLdouble eyey, GLdouble eyez, GLdouble centerx, GLdouble centery, GLdouble centerz, GLdouble upx, GLdouble upy, GLdouble upz)   // 视图变换函数必须在调用任何模型变换函数之前调用，确保首先作用于物体的是模型变换

        glViewport(GLint x,GLint y,GLsizei width,GLsizei height)    // x，y 以像素为单位，指定了视口的左下角位置。width，height 表示这个视口矩形的宽度和高度，根据窗口的实时变化重绘窗口。


        glRotatef(GLfloat angle,GLfloat x,GLfloat y,GLfloat z)  // 该函数用来设置opengl中绘制实体的自转方式，即物体如何旋转

        glGetFloatv(GL_MODELVIEW_MATRIX, mat)   // 取出GL_MODELVIEW_MATRIX，然后存储在mat这个矩阵中，用于逆变换等

        glTexCoordPointer(int size, int type, int stride, Buffer pointer)   // 设置顶点数组为纹理坐标缓存,size：纹理顶点坐标的分量个数,type：纹理坐标的数据类型,stride：位图的宽度，可以理解为相邻的两个纹理之间跨多少个字节，一般为0，因为一般不会在纹理中再添加其他的信息,pointer：存放纹理坐标的数组，指明将绘制的第i个点（i<count）分别对应着贴图的哪一个角，四个角分别用（0,1)（左上角）、(1,1)(右上角)、(1,0)(右下角)、（0，0）(左下角）表示

        void glDrawElements( GLenum mode, GLsizei count,GLenum type, const GLvoid *indices）    // mode指定绘制图元的类型，它应该是下列值之一，GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES, GL_QUAD_STRIP, GL_QUADS, and GL_POLYGON.count为以mode类型连接的顶点的总数，且根据不同的mode，count小于或等于单个mode类型图元的顶点数*图元数。type为索引值的类型，只能是下列值之一：GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, or GL_UNSIGNED_INT。

        glFlush()   //清空缓冲区，将指令送往缓硬件立即执行，但是它是将命令传送完毕之后立即返回，不会等待指令执行完毕。这些指令会在有限时间内执行完毕。如果直接绘制到前缓冲，那么OpenGL的绘制将不会有任何延迟。设想有一个复杂的场景，有很多物体需要绘制。当调用glFlush时，物体会一个一个地出现在屏幕上。但是，如果使用双缓冲，这个函数将不会有什么影响，因为直到交换缓冲区的时候变化才显现出来

