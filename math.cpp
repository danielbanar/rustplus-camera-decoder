#include "math.h"
#define FARPLANE 1023

uint16_t RayDistance(int ray)
{
	return (uint16_t)(ray >> 16);
}

byte RayAlignment(int ray)
{
	return (byte)(ray >> 8);
}

byte RayMaterial(int ray)
{
	return (byte)ray;
}

int Checksum(Ray ray)
{
	auto& distance = ray.m_distance;
	auto& alignment = ray.m_alignment;
	auto& material = ray.m_material;
	return (int)((distance / 128 * 3) + ((uint16_t)(alignment / 16) * 5) + ((uint16_t)(material) * 7) & 63);
}
int Checksum(int distance, int alignment, int material)
{
	return (int)((distance / 128 * 3) + ((uint16_t)(alignment / 16) * 5) + ((uint16_t)(material) * 7) & 63);
}
int RayToInt(Ray ray)
{
	float normalDistance = std::clamp((float)ray.m_distance / FARPLANE, 0.0f, 1.0f);
	uint16_t scaledDistance = (uint16_t)(normalDistance * 1023.0f);
	//Vector3 forward = { 1.0f, 0.0f, 0.0f };
	//Vector3 normal = { -0.8f, 0.2f, 0.0f };
	//float angle = std::max(dot(forward, normal), 0.0f);
	//byte int6angle = (byte)(angle * 63.0f);
	byte int6angle = ray.m_alignment;
	return (scaledDistance >> 8 << 24 | (uint32_t)(scaledDistance & 255) << 16 | (uint32_t)int6angle << 8 | (uint32_t)ray.m_material);
}
int RayToInt(int dist, byte angle, int mat)
{
	float normalDistance = std::clamp((float)dist / FARPLANE, 0.0f, 1.0f);
	uint16_t scaledDistance = (uint16_t)(normalDistance * 1023.0f);
	//Vector3 forward = { 1.0f, 0.0f, 0.0f };
	//Vector3 normal = { -0.8f, 0.2f, 0.0f };
	//float angle = std::max(dot(forward, normal), 0.0f);
	//byte int6angle = (byte)(angle * 63.0f);
	byte int6angle = angle;
	return (scaledDistance >> 8 << 24 | (uint32_t)(scaledDistance & 255) << 16 | (uint32_t)int6angle << 8 | (uint32_t)mat);
}
Ray IntToRay(uint32_t _raw)
{
	auto distance = RayDistance(_raw);
	auto alignment = RayAlignment(_raw);
	auto material = RayMaterial(_raw);
	Ray ray(distance, alignment, material);
	ray.Calculate();
	return ray;
}

Ray HexDecode(std::string hex_string)
{
	if (hex_string.size() != 6)
		std::cout << ERROR("trying to hex decode " + std::to_string(hex_string.size()) + "-digit value, the only valid length is 6");
	std::bitset<24> binary_string(stoul(hex_string, nullptr, 16));
	int unk = binary_string.to_ulong() >> 22;
	int mat = (binary_string.to_ulong() >> 16) & 0x3f;
	int dist = (binary_string.to_ulong() >> 6) & 0x3ff;
	int angle = binary_string.to_ulong() & 0x3f;
	Ray ray((byte)unk, (uint16_t)dist, (byte)angle, (byte)mat);
	ray.Calculate();
	return ray;
}

void Ray::Print()
{
	std::cout << (int)m_distance << "\t " << (int)m_material << "\t " << (int)m_alignment << std::hex << std::uppercase << "\t" << m_raw << '\t' << (int)m_checksum << std::dec << std::nouppercase << std::endl;
	//std::cout << "unk = " << (int)m_unk << ", mat = " << (int)m_material << ", distance = " << (int)m_distance << ", alignment = " << (int)m_alignment << ", checksum = " << (int)m_checksum << '/' << std::hex << std::uppercase << (int)m_checksum << ", int = " << m_raw << std::dec << std::nouppercase << std::endl;
}
void Ray::Calculate()
{
	m_checksum = Checksum(m_distance, m_alignment, m_material);
	m_raw = RayToInt(m_distance, m_alignment, m_material);
}

byte HexToByte(std::string hex_string)
{
	int result;
	std::stringstream ss;
	ss << std::hex << hex_string;
	ss >> result;
	if (ss.fail()) {
		throw std::invalid_argument("Invalid hex string: " + hex_string);
	}
	return result;
}
uint32_t Ray::GetRawRay() const
{
	return m_raw;
}
byte Ray::GetChecksum() const
{
	return m_checksum;
}