#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <vector>
#define WARN(str) "\033[33m" + std::string(str) + "\033[0m"
#define ERROR(str) "\033[31m" + std::string(str) + "\033[0m"
#define NDEBUG
#ifdef NDEBUG
// Release build - define DEBUG to be a no-op
#define DEBUG(str) ((void)0)
#else
// Debug build - define DEBUG as usual
#define DEBUG(str) std::cout << ("\33[36m" + std::string(str) + "\033[0m") << std::endl;
#endif
typedef uint8_t byte;
struct Vector3 
{
	float x, y, z;
};
struct Ray 
{
	/*
	*    1100 0000 1111 1111 1100 0000          0001 0101
	*      |      |            |      |
	*   unk   mat    distance  alignment           checksum
	*   0-2|  2-8 |    8-18    | 18-24|
	*    2     6        10         6                8
	*
	*/
	Ray() : m_unk(0), m_material(0), m_distance(0), m_alignment(0), m_checksum(0), m_raw(0) {}

	Ray(uint8_t unk, uint8_t material, uint16_t distance, uint8_t alignment, uint8_t checksum, uint32_t raw) :
		m_unk(unk), m_material(material), m_distance(distance), m_alignment(alignment), m_checksum(checksum), m_raw(raw) {}

	Ray(uint8_t unk, uint16_t distance, uint8_t alignment, uint8_t material) :
		m_unk(unk), m_material(material), m_distance(distance), m_alignment(alignment), m_checksum(0), m_raw(0) {}

	Ray(uint16_t distance, uint8_t alignment, uint8_t material) :
		m_unk(0), m_material(material), m_distance(distance), m_alignment(alignment), m_checksum(0), m_raw(0) 
	{
		Calculate();
	}
	byte m_unk;
	byte m_material;
	uint16_t m_distance;
	byte m_alignment;
	byte m_checksum;
	uint32_t m_raw;
	void Print();
	void Calculate();
	uint32_t GetRawRay() const;
	byte GetChecksum() const;

};
uint16_t RayDistance(int ray);
byte RayAlignment(int ray);
byte RayMaterial(int ray);
int Checksum(Ray ray);
int Checksum(int distance, int alignment, int material);
int RayToInt(Ray ray);
int RayToInt(int dist, byte angle, int mat);
Ray IntToRay(uint32_t _raw);
Ray HexDecode(std::string hex_string);
byte HexToByte(std::string hex_string);