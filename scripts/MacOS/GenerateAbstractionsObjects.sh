# Compile the Abstractions and generate the oobject files
clang++ -std=c++17 -c -I /Users/phanisrikar/Libraries/soil-master/includes -I /usr/local/Cellar/assimp/5.0.1/include -I vendor/GLM -I vendor/Box2D/include\
        vendor/OpenGL-Abstractions/Camera3D.cpp \
        vendor/OpenGL-Abstractions/CubeMap.cpp \
        vendor/OpenGL-Abstractions/FrameBuffer.cpp \
        vendor/OpenGL-Abstractions/IndexBuffer.cpp \
        vendor/OpenGL-Abstractions/Mesh.cpp \
        vendor/OpenGL-Abstractions/Model.cpp \
        vendor/OpenGL-Abstractions/Rigidbody2D.cpp \
        vendor/OpenGL-Abstractions/Shader.cpp \
        vendor/OpenGL-Abstractions/Texture2D.cpp \
        vendor/OpenGL-Abstractions/RenderBuffer.cpp \
        vendor/OpenGL-Abstractions/VertexArray.cpp \
        vendor/OpenGL-Abstractions/VertexBuffer.cpp

# Remove previously cached files (if exists)
rm -rf bin-int/*.o

# Move the generated object files from root directory to intermediate directory
mv *.o bin-int
