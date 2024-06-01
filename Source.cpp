#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <bitset>

// Узел связного списка
class Node {
public:
    int data;
    Node* next;

    Node(int data) : data(data), next(nullptr) {}
};

// Связный список
class LinkedList {
private:
    Node* head;
public:
    LinkedList() : head(nullptr) {}

    void insert(int data) {
        Node* newNode = new Node(data);
        newNode->next = head;
        head = newNode;
    }

    int remove() {
        if (head == nullptr) {
            throw std::underflow_error("Stack is empty");
        }
        Node* temp = head;
        int data = head->data;
        head = head->next;
        delete temp;
        return data;
    }

    bool isEmpty() const {
        return head == nullptr;
    }
};

// Стек на основе связного списка
class Stack {
private:
    LinkedList list;
public:
    void push(int data) {
        list.insert(data);
    }

    int pop() {
        return list.remove();
    }

    bool isEmpty() const {
        return list.isEmpty();
    }
};

// Двоичный калькулятор
class BinaryCalculator {
private:
    int bitSize;

    void validateInput(int num) {
        int maxValue = (1 << bitSize) - 1; // 2^bitSize - 1
        if (num > maxValue || num < 0) {
            throw std::overflow_error("Input number exceeds bit size limit");
        }
    }

public:
    BinaryCalculator(int bitSize) : bitSize(bitSize) {}

    int add(int a, int b) {
        validateInput(a);
        validateInput(b);
        int result = a + b;
        validateInput(result);
        return result;
    }

    int subtract(int a, int b) {
        validateInput(a);
        validateInput(b);
        int result = a - b;
        if (result < 0) {
            throw std::overflow_error("Result is negative, cannot represent in unsigned binary");
        }
        return result;
    }

    int multiply(int a, int b) {
        validateInput(a);
        validateInput(b);
        int result = a * b;
        validateInput(result);
        return result;
    }

    std::string toBinary(int num) const {
        return std::bitset<32>(num).to_string().substr(32 - bitSize);
    }
};

// Постфиксный калькулятор
class PostfixCalculator {
private:
    Stack stack;
    BinaryCalculator calculator;

    bool isOperator(const std::string& token) const {
        return token == "+" || token == "-" || token == "*";
    }

    int performOperation(const std::string& operation, int operand1, int operand2) {
        if (operation == "+") {
            return calculator.add(operand1, operand2);
        }
        else if (operation == "-") {
            return calculator.subtract(operand1, operand2);
        }
        else if (operation == "*") {
            return calculator.multiply(operand1, operand2);
        }
        else {
            throw std::invalid_argument("Invalid operator");
        }
    }

public:
    PostfixCalculator(int bitSize) : calculator(bitSize) {}

    void evaluate(const std::string& expression) {
        std::istringstream tokens(expression);
        std::string token;
        while (tokens >> token) {
            if (isdigit(token[0])) {
                stack.push(std::stoi(token));
            }
            else if (isOperator(token)) {
                if (stack.isEmpty()) {
                    throw std::invalid_argument("Invalid postfix expression");
                }
                int operand2 = stack.pop();
                if (stack.isEmpty()) {
                    throw std::invalid_argument("Invalid postfix expression");
                }
                int operand1 = stack.pop();
                int result = performOperation(token, operand1, operand2);
                stack.push(result);
            }
            else {
                throw std::invalid_argument("Invalid token in postfix expression");
            }
        }
        if (!stack.isEmpty()) {
            int result = stack.pop();
            if (!stack.isEmpty()) {
                throw std::invalid_argument("Invalid postfix expression");
            }
            std::cout << "Decimal result: " << result << std::endl;
            std::cout << "Binary result: " << calculator.toBinary(result) << std::endl;
        }
    }
};

int main() {
    int bitSize;
    std::cout << "Enter the bit size: ";
    std::cin >> bitSize;
    std::cin.ignore();

    PostfixCalculator calculator(bitSize);

    std::string expression;
    std::cout << "Enter the postfix expression: ";
    std::getline(std::cin, expression);

    try {
        calculator.evaluate(expression);
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
