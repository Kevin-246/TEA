//
//  main.cpp
//  TinyEncryption
//
//  Created by CodingKevin on 2024-01-23.
//

#include <iostream>

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

int main(int argc, const char * argv[]) {
    // insert code here...
    uint32_t v[2] = {1, 2};
    uint32_t k[4] = {3, 4, 5, 6};
    
    TEA tea(k);
    std::cout << "Plain text: " << v[0] << ", " << v[1] << std::endl;
    tea.encrypt(v);
    std::cout << "Cipher text: " << v[0] << ", " << v[1] << std::endl;
    tea.decrypt(v);
    std::cout << "Deciphed text: " << v[0] << ", " << v[1] << std::endl;
    return 0;
}
