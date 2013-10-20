JotunEngine2: Camera.o InputHandler.o main.o Mesh.o Shader.o stb_image-1.16.o Texture.o Time.o vboindexer.o
	g++ Camera.o InputHandler.o main.o Mesh.o Shader.o stb_image-1.16.o Texture.o Time.o vboindexer.o -o JotunEngine2 -lglfw3 -lglew32 -lopengl32 -lglu32

Camera.o: Camera.cpp
	g++ -c Camera.cpp

ImputHandler.o: InputHandler.cpp
	g++ -c InputHandler.o

main.o: main.cpp
	g++ -c main.cpp

Mesh.o: Mesh.cpp
	g++ -c Mesh.cpp

Shader.o: Shader.cpp
	g++ -c Shader.cpp

stb_image-1.16.o: stb_image-1.16.cpp
	g++ -c stb_image-1.16.cpp

Texture.o: Texture.cpp
	g++ -c Texture.cpp

Time.o: Time.cpp
	g++ -c Time.cpp

vboindexer.o: vboindexer.cpp
	g++ -c vboindexer.cpp

clean:
	rm *o JotunEngine2
