//
//  main.cpp
//  TinyEncryption
//
//  Created by CodingKevin on 2024-01-23.
//

#include <iostream>
#include <cstdint>
#include <vector>

/// An implementation of the TEA (Tiny Encryption Algorithm) with a key size of 128 bits.
class TEA{
private:
    uint32_t key[4];
    
    const uint32_t delta = 0x9E3779B9;
public:
    TEA(uint32_t key[4]){
        for(int i = 0; i < 4; i++){
            this -> key[i] = key[i];
        }
    }

    void encrypt(uint32_t v[2]){
        uint32_t v0 = v[0], v1 = v[1], sum = 0;
        uint32_t k0 = key[0], k1 = key[1], k2 = key[2], k3 = key[3];
        
        for(uint32_t i = 0; i < 32; i++){
            sum += delta;
            v0 += ((v1 << 4) + k0) ^ (v1 + sum) ^ ((v1 >> 5) + k1);
            v1 += ((v0 << 4) + k2) ^ (v0 + sum) ^ ((v0 >> 5) + k3);
        }
        v[0] = v0;
        v[1] = v1;
    }
    
    void decrypt(uint32_t v[2]){
        uint32_t v0 = v[0], v1 = v[1], sum = 0xC6EF3720;
        uint32_t k0 = key[0], k1 = key[1], k2 = key[2], k3 = key[3];
        
        for(uint32_t i = 0; i < 32; i++){
            v1 -= ((v0<<4) + k2) ^ (v0 + sum) ^ ((v0>>5) + k3);
            v0 -= ((v1<<4) + k0) ^ (v1 + sum) ^ ((v1>>5) + k1);
            sum -= delta;
        }
        v[0] = v0;
        v[1] = v1;
    }
};

std::vector<uint32_t> setBlock(std::string a){
    std::vector<uint32_t> block;
    uint8_t byte1 = 0, byte2 = 0, byte3 = 0, byte4 = 0;
    uint32_t value = 0;
    
    if(a.length() < 4){
        switch(a.length()){
            case 1:
                byte1 = a[0];
                break;
            case 2:
                byte1 = a[0];
                byte2 = a[1];
                break;
            case 3:
                byte1 = a[0];
                byte2 = a[1];
                byte3 = a[2];
                break;
            default:
                return block;
                break;
        }
        value = (byte1 << 24) | (byte2 << 16) | (byte3 << 8);
        block.push_back(value);
        return block;
    }else{
        for(int i = 0; i < a.length() - 4; i += 4){
            byte1 = a[i];
            byte2 = a[i + 1];
            byte3 = a[i + 2];
            byte4 = a[i + 3];
            value = (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | (byte4);
            block.push_back(value);
        }
        switch(a.length() % 4){
            case 1:
                byte1 = a[(a.length() - a.length() % 4) + 0];
                byte2 = 0;
                byte3 = 0;
                break;
            case 2:
                byte1 = a[(a.length() - a.length() % 4) + 0];
                byte2 = a[(a.length() - a.length() % 4) + 1];
                byte3 = 0;
                break;
            case 3:
                byte1 = a[(a.length() - a.length() % 4) + 0];
                byte2 = a[(a.length() - a.length() % 4) + 1];
                byte3 = a[(a.length() - a.length() % 4) + 2];
                break;
            default:
                return block;
                break;
        }
        value = 0;
        value = (byte1 << 24) | (byte2 << 16) | (byte3 << 8);
        block.push_back(value);
    }
    return block;
}

std::string retBlock(std::vector<uint32_t> a){
    std::string str;
    uint8_t byte1, byte2, byte3, byte4;
    
    for(int i = 0; i < a.size(); i++){
        byte1 = (a[i] >> 24) & 0xFF;
        str.push_back(byte1);
        if(byte1 == 0) break;
        
        byte2 = (a[i] >> 16) & 0xFF;
        str.push_back(byte2);
        if(byte2 == 0) break;
        
        byte3 = (a[i] >> 8) & 0xFF;
        str.push_back(byte3);
        if(byte3 == 0) break;
        
        byte4 = (a[i]) & 0xFF;
        str.push_back(byte4);
        if(byte4 == 0) break;
    }
    
    return str;
}

int main(int argc, const char * argv[]) {
    // insert code here...
    std::string input = "ABCDEFGHI";
    std::vector<uint32_t> block = setBlock(input);
    uint32_t key[4] = {1, 2, 3, 4};
    if(block.size() % 2 != 0){
        block.push_back(0);
    }
    std::cout << "Original data: ";
    std::cout << input << std::endl;
    
    TEA tea(key);
    
    for(int i = 0; i < block.size(); i += 2){
        tea.encrypt((block.data()) + i);
    }
    
    // I coudn't figure out how to print the string correctly so I'm just going to print the message blocks
    std::cout << "Encrypted block data: ";
    for(int i = 0; i < block.size(); i++){
        std::cout << block[i] << " ";
    }
    std::cout << std::endl;
    
    for(int i = 0; i < block.size(); i += 2){
        tea.decrypt((block.data()) + i);
    }
    
    std::cout << "Decrypted data: ";
    std::cout << retBlock(block) << std::endl;
    return 0;
}
