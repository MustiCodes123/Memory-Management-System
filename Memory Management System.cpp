#include <iostream>
#include <string>
using namespace std;
template<class T>
class List;
class block;
class Program;

class block
{
    template<class T>
    friend class List;
    int start_byte_ID;
    int total_bytes;

public:
    block()
    {}

    block(int start, int byte)
    {
        start_byte_ID = start;
        total_bytes = byte;
    }
    ~block()
    {

    }
};
template <class T>
class List {
public:
    List()
    {
        head = new Node();
        tail = new Node();
    }
    List(const List& obj)
    {
        this->head = obj.head;
        this->tail = obj.tail;
    }
    /*List& operator=(const List& obj)
    {
        Node* temp = obj.head;
        while (temp != tail)
        {
            this->head = obj.head;
            this->tail = obj.tail;
        }
        return *this;
    }*/
    void insertAtStart(T* obj)
    {
        if (head->next)
        {
            Node* temp = new Node(obj, head->next);
            head->next = temp;
        }
        else
        {
            head->next = new Node(obj, tail);
            tail->next = NULL;
        }
    }
    void insertAtEnd(T* obj)
    {
        if (head->next == 0) {
            head->next = new Node(obj, tail);
            tail->next = NULL;
        }
        else
        {
            Node* temp = head;
            while (temp->next != tail)
                temp = temp->next;
            temp->next = new Node(obj, tail);
        }

    }
    void PrintPool()
    {

        Node* temp = head->next;
        while (temp != tail) {
            cout << "Start Byte ID: " << temp->data.start_byte_ID << "\tTotal Bytes: " << temp->data.total_bytes << endl;
            temp = temp->next;
        }
        cout << endl;
    }
    void PrintProgs()
    {
        Node* temp = head->next;
        while (temp != tail)
        {
            cout << "Program ID: " << temp->data.ID << "\tTotal bytes: " << temp->data.size << endl;
            cout << "blocks of " << temp->data.ID << ": " << endl;
            temp->data.PrintBlocks();
            temp = temp->next;

        }
        cout << endl;
    }
    void AddBlockToProg(string ID, block* blk)
    {
        Node* temp = head->next;

        while (temp != tail)
        {
            if (temp->data.ID == ID)
            {
                temp->data.AddMemory(blk);
            }
            temp = temp->next;
        }
    }
    bool isEmpty() const
    {
        return head->next == 0;
    }
    bool PoolCheck(int bytes)
    {
        Node* temp = head->next;
        while (temp != tail && temp != NULL)
        {
            if (temp->data.total_bytes >= bytes)
            {
                return true;
            }
            temp = temp->next;
        }
        return false;
    }
    void DeleteProgs(string id, List <block>& pool)
    {
        Node* temp = head->next;
        Node* Del = head;
        List<block>* returnBlocks = new List<block>;
        while (temp != tail)
        {
            if (temp->data.ID == id)
            {
                //*returnBlocks = temp->data.ListOfBlocks();
                pool.ReturnMemoryToPool(&temp->data.ListOfBlocks());
                Del->next = temp->next;
                delete temp;
                break;
            }
            temp = temp->next;
            Del = Del->next;
        }
    }
    void SaveBlocks(List<block>& b)
    {
        Node* temp = b.head;
        while (temp != b.tail->next)
        {
            head = new Node(&temp->data, head->next);
            temp = temp->next;
        }
    }
    void ReturnMemoryToPool(List<block>* blocks)
    {
        Node* temp = blocks->head->next;

        while (temp != blocks->tail)
        {
            int start, byte;
            start = temp->data.start_byte_ID;
            byte = temp->data.total_bytes;
            block* temp1 = new block(start, byte);
            this->insertAtEnd(temp1);
            temp = temp->next;
        }
    }
    bool SearchID(string ID)
    {
        Node* temp = head->next;
        while (temp != tail && temp != NULL)
        {
            if (temp->data.ID == ID)
            {
                return true;
            }
            temp = temp->next;
        }
        return false;
    }
    int GetMemoryFromPool(int& ReqBytes, int Strategy)
    {

        int Startbytes = 0;
        if (Strategy == 1)   // First Fit
        {
            Node* temp = head->next;
            while (temp != tail)
            {

                if (temp->data.total_bytes >= ReqBytes)
                {
                    Startbytes = temp->data.start_byte_ID;
                    temp->data.total_bytes = temp->data.total_bytes - ReqBytes;
                    temp->data.start_byte_ID = temp->data.start_byte_ID + ReqBytes;
                    return Startbytes;
                }

                temp = temp->next;
            }
        }
        else if (Strategy == 0) // Best Fit
        {
            Node* temp = head->next;
            int Min = temp->data.total_bytes;
            int count = 0;

            while (temp != tail)
            {
                if (temp->data.total_bytes < Min && temp->data.total_bytes >= ReqBytes) // find minimum bytes in pool
                {                                                                       // and store in Min
                    Min = temp->data.total_bytes;
                    count++;
                }
                temp = temp->next;
            }
            if (count == 0)//if upper condition doesnt works, get memory from the bigger block
            {
                temp = head->next;
                while (temp != tail)
                {
                    if (temp->data.total_bytes >= ReqBytes)
                    {
                        Min = temp->data.total_bytes;
                        count++;
                    }
                    temp = temp->next;
                }
            }
            temp = head->next;
            while (temp != tail)
            {
                if (temp->data.total_bytes == Min)  //traverse till that block to perform the operation
                {
                    Startbytes = temp->data.start_byte_ID;
                    temp->data.total_bytes = temp->data.total_bytes - ReqBytes;
                    temp->data.start_byte_ID = temp->data.start_byte_ID + ReqBytes;
                    break;
                }
                temp = temp->next;
            }

            temp = head->next;
            Node* NextNode = head->next->next;

            while (NextNode != tail)//loop for deleting the block with 0 bytes 
            {
                if (NextNode->data.total_bytes == 0)
                {
                    temp->next = NextNode->next;
                    delete NextNode;
                    break;
                }
                temp = temp->next;
                NextNode = NextNode->next;
            }

            return Startbytes;

        }
    }
    void UpdateTotalBytes(string id)
    {
        int total = 0;
        Node* temp = head->next;
        while (temp != tail) {

            if (temp->data.ID == id)
            {
                temp->data.size = temp->data.TotalBytes(total);
            }
            temp = temp->next;
        }
    }
    int CalculateTotal(int total)
    {
        Node* temp = head->next;
        while (temp != tail)
        {
            total = total + temp->data.total_bytes;
            temp = temp->next;
        }
        return total;
    }
    void Merge()
    {
        Node* temp = head->next;
        Node* NextNode = head->next->next;
        if (NextNode == NULL)
            return;
        while (NextNode != tail)
        {
            if ((temp->data.start_byte_ID + temp->data.total_bytes) == NextNode->data.start_byte_ID)
            {
                temp->data.total_bytes = temp->data.total_bytes + NextNode->data.total_bytes;
                temp->next = NextNode->next;
                delete NextNode;
                NextNode = temp->next;
            }
            else
            {
                temp = temp->next;
                NextNode = NextNode->next;
            }
        }
    }
    void Sort()
    {
        //Node current will point to head  
        Node* current = head->next, * index = NULL;
        int tempStart;
        int tempSize;
        if (head == tail) {
            return;
        }
        else {
            while (current != tail) {
                //Node index will point to node next to current  
                index = current->next;

                while (index != tail) {
                    //If current node's data is greater than index's node data, swap the data between them  
                    if (current->data.start_byte_ID > index->data.start_byte_ID) {
                        tempStart = current->data.start_byte_ID;
                        tempSize = current->data.total_bytes;
                        current->data.start_byte_ID = index->data.start_byte_ID;
                        current->data.total_bytes = index->data.total_bytes;
                        index->data.start_byte_ID = tempStart;
                        index->data.total_bytes = tempSize;
                    }
                    index = index->next;
                }
                current = current->next;
            }
        }


    }
    ~List()
    {
        Node* temp;
        while (head->next != NULL)
        {
            temp = head;
            head = head->next;
            delete temp;
        }
        delete head->next;
    }

private:
    struct Node
    {
        T data;
        Node* next;

        Node()
        {
            next = NULL;
        }
        Node(T* el, Node* ptr = 0)
        {
            data = *el;
            next = ptr;
        }
    };
    Node* head;
    Node* tail;
};
class Program {

    string ID;
    int size;
    template <class T>
    friend class List;
    List<block> blocks;

public:

    Program()
    {}

    Program(string id, int s, block* blk)
    {
        ID = id;
        size = s;
        blocks.insertAtEnd(blk);
    }
    void AddMemory(block* blk)
    {
        blocks.insertAtEnd(blk);
    }
    List<block>& ListOfBlocks()
    {
        return blocks;
    }
    void PrintBlocks()
    {
        blocks.PrintPool();
    }
    int TotalBytes(int t)
    {
        t = blocks.CalculateTotal(t);
        return t;
    }
    ~Program()
    {

    }
};
template <class T>
class MemoryManagementSystem
{

    List <block> pool;
    List <Program> Progs;
    bool Strategy;
    int SizeOfMemory;

public:

    MemoryManagementSystem(int size, int str)
    {
        SizeOfMemory = size;
        Strategy = str;
    }
    bool GetMem(string id, int reqSize)
    {
        int ProgramStartByte = 0;

        if (Progs.SearchID(id) != 0 && pool.PoolCheck(reqSize) != 0)
        {
            ProgramStartByte = pool.GetMemoryFromPool(reqSize, Strategy);
            block* temp = new block(ProgramStartByte, reqSize);
            Progs.AddBlockToProg(id, temp);
            Progs.UpdateTotalBytes(id);
            return true;
        }
        else if (Progs.SearchID(id) == 0 && pool.PoolCheck(reqSize) != 0)    // if id doesnt exists  
            // and there exists free space in pool then make a new program
        {
            ProgramStartByte = pool.GetMemoryFromPool(reqSize, Strategy);
            block* temp = new block(ProgramStartByte, reqSize);

            Program* temp2 = new Program(id, reqSize, temp);
            Progs.insertAtEnd(temp2);

            return true;

        }
        else    //no free space in pool
            return false;

    }
    void addBlockToPool(T const start, T const size)
    {
        block* temp = new block(start, size);
        pool.insertAtStart(temp);
    }
    bool DeleteProgram(string id)
    {
        if (Progs.SearchID(id) != 0)
        {
            List<block>* returnBlocks = new List<block>;
            Progs.DeleteProgs(id, pool);

            pool.Sort();
            pool.Merge();

            return true;

        }
        else
            return false;
    }
    void printMemoryPool()
    {
        pool.PrintPool();
    }
    void printProgs()
    {
        Progs.PrintProgs();
    }
};
int main()
{
    bool strategy = false;
    bool run = true;
    int RAM = 0;
    int input = 0;

    cout << "Enter True (1) for First Come Strategy && False (0) for Best Fit Strategy" << endl;
    cin >> strategy;
    cout << endl;

    cout << "Enter RAM size" << endl;
    cin >> RAM;

    MemoryManagementSystem<int> object(RAM, strategy);
    object.addBlockToPool(0, RAM);

    while (run == true)
    {
        cout << endl;
        cout << "Press 1 to enter a program" << endl;
        cout << "Press 2 to delete a program" << endl;
        cout << "Press 3 to view the Pool (Memory of free blocks)" << endl;
        cout << "Press 4 to view the Programs in Memory" << endl;
        cout << "Press 5 to exit the program" << endl;
        cout << endl;

        cin >> input;

        if (input == 1)
        {
            char* programId = new char[8];
            int memoryRequired = 0;
            cout << "Enter Program ID" << endl;
            cin >> programId;
            cout << "Enter memory required for the program" << endl;
            cin >> memoryRequired;
            if (object.GetMem(programId, memoryRequired))
            {
                cout << "Memory succesfully allocated" << endl;
            }
            else
            {
                cout << "Memory not available" << endl;
            }
        }
        else if (input == 2)
        {
            char* programId = new char[8];
            cout << "Enter Program ID which you wish to delete for eg P1" << endl;
            cin >> programId;
            if (object.DeleteProgram(programId))
            {
                cout << "Memory succesfully deallocated" << endl;
            }
            else
            {
                cout << "Memory can't be deallocated. Program does not exist" << endl;
            }
        }
        else if (input == 3)
        {
            object.printMemoryPool();
        }
        else if (input == 4)
        {
            object.printProgs();
        }
        else if (input == 5)
        {
            run = false;
            cout << "Program exited." << endl;
        }
        else
        {
            cout << "Invalid Input. Enter again" << endl;
        }
    }
}