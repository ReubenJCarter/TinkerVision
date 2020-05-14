#pragma once

namespace TnkrVis
{

class ShaderStorageBuffer
{
    public:
		enum USAGE{STREAM, STATIC, DYNAMIC};

	private:
        USAGE useage; 
		unsigned int size;
		unsigned int storageBuffer;
		unsigned int bindingPoint;
		
	public:
		ShaderStorageBuffer();
		~ShaderStorageBuffer();
		void Allocate(unsigned int sz, unsigned int bindingP, unsigned char* data=0, USAGE u=STATIC);
		void Deallocate();
		void Copy(unsigned char* data, USAGE u=STATIC);
		void Get(unsigned char* data);
		unsigned int GetBindingPoint();		
		unsigned int Size();
};

}
