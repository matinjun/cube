# 窗口
## 配置
**Note**: 所有的环境都在**64位**下运行，**注意**
1. 确保有CMake，vs2019Comunity
2. [glfw3](https://www.glfw.org/download.html)，下载sourcepackage编译。<br>
[glad](https://glad.dav1d.de/)网站会按照要求自动生成配置文件。<br>
编译过程与生成细节具体细节见[learnopengl](https://learnopengl.com/Getting-started/Creating-a-window)。<br>
这步你会手动编译开源软件，一定要参考教材。
3. 最后需要配置vc++目录，同样[learnopengl](https://learnopengl.com/Getting-started/Creating-a-window)
## 代码部分
分为三个部分
- Init()部分，初始化必要的资源
- 创建窗口等对象，回调函数
- 渲染循环，实际上还要加上一个结束部分（释放资源glfwTerminate()）
## 基础知识
### vao, vbo, shader
1. opengl使用流水线，将三维空间的点变为二维窗口的点，这之中需要六个流水部分；每个部分就是一个在GPU上运行的程序，叫做shader，我们需要完成的是vertex shader与fragrament shader，具体[参考](https://learnopengl.com/Getting-started/Hello-Triangle)
    1. 顶点着色器可以理解为对每个顶点进行处理
    2. 片元着色器可以理解为对每个像素的处理
2. 坐标，VBO
    1. 坐标(x, y, z)分量只能在[-1, 1]之间，超出会被裁剪，此时需要投影变换
    2. VBO指GPU中的缓存对象，通过索引号VBO来访问。使用glGenBuffers(num, &arrayVBO)，产生num个VBO索引到arrayVBO上。
     - VBO有类型，通过```glBindBuffer(GL_ARRAY_BUFFER, VBOIndex); ```绑定某个VBO对象的类型
     - 通过```glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);```将内存中的数据复制到GPU缓存中
        * 第一个参数指传递目标缓存的类型
        * 第二个参数指传递内存的大小
        * 第三个指被传递数据的起始地址
        * 第四个指显卡管理从内存传来的数据的方式，是改变还是不变等等
            - ```GL_STREAM_DRAW```: the data is set only once and used by the GPU at most a few times.
            - ```GL_STATIC_DRAW```: the data is set only once and used many times.
            - ```GL_DYNAMIC_DRAW```: the data is changed a lot and used many times.
    3. shader（着色器）
     - shader是流水线的一部分，用于处理输入的数据并输出
     - 我们只需要实现vertex shader与fragment shader部分
```c++
// 产生shader对象，并返回索引
unsigned int vertexShader;
vertexShader = glCreateShader(GL_VERTEX_SHADER); // 此处的参数是指shader的种类

glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // 将shader与某个代码片段联系起来
glCompileShader(vertexShader); // 编译

unsigned int fragmentShader;
fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
glCompileShader(fragmentShader);

// 产生程序对象，并将编译好的shader与它连接起来
unsigned int shaderProgram;
shaderProgram = glCreateProgram();

glAttachShader(shaderProgram, vertexShader);
glAttachShader(shaderProgram, fragmentShader);
glLinkProgram(shaderProgram);

// 使用这个程序
glUseProgram(shaderProgram);

// 之后shader不再使用，对于不使用的shader对象，需要删除
glDeleteShader(vertexShader);
glDeleteShader(fragmentShader);
```
3. VAO是指“存储vbo绑定的属性”**绘制以VAO为单位**
    - 属性指如何解释GPU中的缓存，即以什么方式访问缓存
    - 关于缓存的属性```glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);```解释了如何访问顶点缓存
       * 第一个指我们先要配置哪个顶点属性，即顶点属性位置
       * 第二个解释了每个顶点的属性个数，如x y z
       * 第三个指每个属性的类型
       * 第四个默认为GL_FALSE
       * 第五个指一个顶点的大小
       * 第六个指相对于缓存起始的偏移子节数
    - 激活属性```glEnableVertexAttribArray(0)```参数为顶点属性位置
    - 以上其实是改变了某个状态，VAO对象负责负责存储这些状态，即与相应的VBO状态绑定（VBO的属性，数据等）
       * 要使用VAO，就要再先要存储的缓存对象前生成并绑定类型，之后绘制时再重新绑定，代码如下：
       * 绑定了VAO时，从那个点开始的任何后续顶点属性调用都将存储在VAO中
```c++
// ..:: Initialization code (done once (unless your object frequently changes)) :: ..

// 0. generate VAO
unsigned int VAO;
glGenVertexArrays(1, &VAO);

// 1. bind Vertex Array Object
glBindVertexArray(VAO);
// 2. copy our vertices array in a buffer for OpenGL to use
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
// 3. then set our vertex attributes pointers
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);  

  
[...]

// ..:: Drawing code (in render loop) :: ..
// 4. draw the object
glUseProgram(shaderProgram);
glBindVertexArray(VAO);
someOpenGLFunctionThatDrawsOurTriangle();   
```

4. 关于EBO
由于opengl是以三角面片为单元绘制，当绘制一个多边形时会出现多个点复用的情况，为节省空间，使用EBO缓存对象在GPU中存索引，具体用法与VBO类似
```c++
// ..:: Initialization code :: ..
// 1. bind Vertex Array Object
glBindVertexArray(VAO);
// 2. copy our vertices array in a vertex buffer for OpenGL to use
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
// 3. copy our index array in a element buffer for OpenGL to use
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
// 4. then set the vertex attributes pointers
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);  

[...]
  
// ..:: Drawing code (in render loop) :: ..
glUseProgram(shaderProgram);
glBindVertexArray(VAO);
glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0)
glBindVertexArray(0);
```

5, 关于解绑，不使用时，解绑如下
```c++
	// 解绑相应VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// 解绑相应VAO
	glBindVertexArray(0);
```

<HR>

### 关于更多解释以及例子
1. [多个三角形绘制](https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/2.2.hello_triangle_indexed/hello_triangle_indexed.cpp)
2. [使用多个VAO, VBO](https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/2.4.hello_triangle_exercise2/hello_triangle_exercise2.cpp)
3. [使用多个shader绘制不同的颜色](https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/2.5.hello_triangle_exercise3/hello_triangle_exercise3.cpp)

### 小总结
代码框架
1. init
2. 窗口初始化
3. 编译shader
4. 通过VBO传送数据到GPU，并描述，使用VAO分离不同的数据
5. 循环渲染
6. 释放


## shader精细解答
1. in, out, uniform
    - in: 从外部输入的数据，每个从外部输入的数据都有位置索引，可以通过layout显示指定，或是通过函数询问获得位置索引
    - out: 输出到下一级流水线的数据，其中frag shader必须输出一个四维颜色向量，vertex shader必须有一个输入，并且要输出到gl_Position_
    - uniform: 全局，一定义就可以通过在渲染循环中传递值，从而动态改变点或数据
```c++
// update the uniform color
float timeValue = glfwGetTime();
float greenValue = sin(timeValue) / 2.0f + 0.5f;
// 获得索引
int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

// now render the triangle
glBindVertexArray(VAO); // 选择要绘制的VAO
glDrawArrays(GL_TRIANGLES, 0, 3); // 绘制

```
- in, out在不同的shader有相同的名字时可以相互调用（注意顺序，先out后in），即相当于同一个变量
- glVertexAttribPointer()中第一个参数就是指在shader中的索引，倒数第二个指每次shader读取的长度

## 纹理
1. 纹理坐标是从[0, 1]超出范围有几种处理方法
    - GL_REPEAT
    - GL_MIRRORED_REPEAT
    - GL_CLAMP_TO_EDGE
    - GL_CLAMP_TO_BORDER
    具体参考[learnopengl](https://learnopengl.com/Getting-started/Textures)
2. 纹理采样(texture wrapping)
    - GL_NEAREST 取最靠近纹理标架的采样
    - GL_LINEAR 取靠近纹理标架的线性平均
3. Mipmaps，对于远距离的采样，为了不浪费cache，需要使用mipmap确保远处物体纹理渲染强度小(filter)
    - 使用mipmap时，就别使用放大滤波选项，因为mipmap本身就是2倍减小的一系列纹理
4. 生成纹理对象与绑定产生图片
```c++
    // load and create a texture 
    // -------------------------
    unsigned int texture[2];
    glGenTextures(2, texture);
    glBindTexture(GL_TEXTURE_2D, texture[0]); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char* data = stbi_load("resources/textures/container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        // 对于倒数第三个参数，如果是png图片，参数应该是GL_RGBA，因为.png格式有透明度
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    [...]
```
5. shader.fs
```c
#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D texturel;
uniform sampler2D texture2;

void main()
{
	FragColor = mix(texture(texturel, TexCoord), texture(texture2, TexCoord), 0.2) * vec4(ourColor, 1.0);
}
```
此时，在main文件通过相应的函数获得texture1，texture2在shader中的索引，并分别绑定纹理对象
注意，opengl至少可以支持16个纹理对象，GL_TEXTURE0 --- GL_TEXTURE15
```c++
ourShader.use(); // don't forget to activate the shader before setting uniforms!  
glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0); // set it manually
ourShader.setInt("texture2", 1); // or with shader class
// bind Texture
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, texture[0]);
glActiveTexture(GL_TEXTURE1);
glBindTexture(GL_TEXTURE_2D, texture[1]);
```
