#pragma once

class MemoryManager{
    public:
        static MemoryManager* getInstance();
		static void initialize();
		static void destroy();
    private:
        MemoryManager();
        static MemoryManager* instance;
        



};