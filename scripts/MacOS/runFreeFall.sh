# Compile the file
clang++ -std=c++17 -c -I /Users/phanisrikar/Libraries/soil-master/includes -I /Users/phanisrikar/Libraries -I /usr/local/Cellar/assimp/5.0.1/include -I vendor/GLM -I vendor/Box2D/include -I vendor/OpenGL-Abstractions/ \
        src/Free\ Fall/FreeFall.cpp

mv *.o bin-int

#Link it with the libraries and other objects
clang++ -std=c++17 -L vendor/Box2D/lib -L /usr/local/Cellar/assimp/5.0.1/lib -L /Users/phanisrikar/Libraries/soil-master/build \
        -framework OpenGL -framework CoreFoundation -lglew -lglfw -lbox2d -lsoil -lassimp \
        -o FreeFall \
        bin-int/FreeFall.o \
        bin-int/Camera3D.o \
        bin-int/CubeMap.o \
        bin-int/FrameBuffer.o \
        bin-int/IndexBuffer.o \
        bin-int/Mesh.o \
        bin-int/Model.o \
        bin-int/Rigidbody2D.o \
        bin-int/Shader.o \
        bin-int/Texture2D.o \
        bin-int/RenderBuffer.o \
        bin-int/VertexArray.o \
        bin-int/VertexBuffer.o

# Run the Executable
mv FreeFall bin/
./bin/FreeFall
