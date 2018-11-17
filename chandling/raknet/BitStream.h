/*
	This was taken from: https://github.com/BlastHackNet/mod_s0beit_sa/
*/
#pragma once

#include <assert.h>
#/// Given a number of bits, return how many bytes are needed to represent that.
#define BITS_TO_BYTES(x) (((x)+7)>>3)
#define BYTES_TO_BITS(x) ((x)<<3)
#define BITSTREAM_STACK_ALLOCATION_SIZE 256

/// This class allows you to write and read native types as a string of bits.  BitStream is used extensively throughout RakNet and is designed to be used by users as well.	
/// \sa BitStreamSample.txt
class BitStream
{

public:
	/// Default Constructor 
	BitStream();

	/// Create the bitstream, with some number of bytes to immediately allocate.
	/// There is no benefit to calling this, unless you know exactly how many bytes you need and it is greater than BITSTREAM_STACK_ALLOCATION_SIZE.
	/// In that case all it does is save you one or more realloc calls.
	/// \param[in] initialBytesToAllocate the number of bytes to pre-allocate. 
	BitStream(int initialBytesToAllocate);

	/// Initialize the BitStream, immediately setting the data it contains to a predefined pointer.
	/// Set \a _copyData to true if you want to make an internal copy of the data you are passing. Set it to false to just save a pointer to the data.
	/// You shouldn't call Write functions with \a _copyData as false, as this will write to unallocated memory
	/// 99% of the time you will use this function to cast Packet::data to a bitstream for reading, in which case you should write something as follows:
	/// \code
	/// RakNet::BitStream bs(packet->data, packet->length, false);
	/// \endcode
	/// \param[in] _data An array of bytes.
	/// \param[in] lengthInBytes Size of the \a _data.
	/// \param[in] _copyData true or false to make a copy of \a _data or not.
	BitStream(unsigned char* _data, unsigned int lengthInBytes, bool _copyData);

	/// Destructor 
	~BitStream();

	/// Resets the bitstream for reuse.
	void Reset(void);

	/// Bidirectional serialize/deserialize any integral type to/from a bitstream.  Undefine __BITSTREAM_NATIVE_END if you need endian swapping.
	/// \param[in] writeToBitstream true to write from your data to this bitstream.  False to read from this bitstream and write to your data
	/// \param[in] var The value to write
	/// \return true if \a writeToBitstream is true.  true if \a writeToBitstream is false and the read was successful.  false if \a writeToBitstream is false and the read was not successful.
	template <class templateType>
	bool Serialize(bool writeToBitstream, templateType &var);

	/// Bidirectional serialize/deserialize any integral type to/from a bitstream.  If the current value is different from the last value
	/// the current value will be written.  Otherwise, a single bit will be written
	/// \param[in] writeToBitstream true to write from your data to this bitstream.  False to read from this bitstream and write to your data
	/// \param[in] currentValue The current value to write
	/// \param[in] lastValue The last value to compare against.  Only used if \a writeToBitstream is true.
	/// \return true if \a writeToBitstream is true.  true if \a writeToBitstream is false and the read was successful.  false if \a writeToBitstream is false and the read was not successful.
	template <class templateType>
	bool SerializeDelta(bool writeToBitstream, templateType &currentValue, templateType lastValue);

	/// Bidirectional version of SerializeDelta when you don't know what the last value is, or there is no last value.
	/// \param[in] writeToBitstream true to write from your data to this bitstream.  False to read from this bitstream and write to your data
	/// \param[in] currentValue The current value to write
	/// \return true if \a writeToBitstream is true.  true if \a writeToBitstream is false and the read was successful.  false if \a writeToBitstream is false and the read was not successful.
	template <class templateType>
	bool SerializeDelta(bool writeToBitstream, templateType &currentValue);

	/// Bidirectional serialize/deserialize any integral type to/from a bitstream.  Undefine __BITSTREAM_NATIVE_END if you need endian swapping.
	/// If you are not using __BITSTREAM_NATIVE_END the opposite is true for types larger than 1 byte
	/// For floating point, this is lossy, using 2 bytes for a float and 4 for a double.  The range must be between -1 and +1.
	/// For non-floating point, this is lossless, but only has benefit if you use less than half the range of the type
	/// \param[in] writeToBitstream true to write from your data to this bitstream.  False to read from this bitstream and write to your data
	/// \param[in] var The value to write
	/// \return true if \a writeToBitstream is true.  true if \a writeToBitstream is false and the read was successful.  false if \a writeToBitstream is false and the read was not successful.
	template <class templateType>
	bool SerializeCompressed(bool writeToBitstream, templateType &var);

	/// Bidirectional serialize/deserialize any integral type to/from a bitstream.  If the current value is different from the last value
	/// the current value will be written.  Otherwise, a single bit will be written
	/// For floating point, this is lossy, using 2 bytes for a float and 4 for a double.  The range must be between -1 and +1.
	/// For non-floating point, this is lossless, but only has benefit if you use less than half the range of the type
	/// If you are not using __BITSTREAM_NATIVE_END the opposite is true for types larger than 1 byte
	/// \param[in] writeToBitstream true to write from your data to this bitstream.  False to read from this bitstream and write to your data
	/// \param[in] currentValue The current value to write
	/// \param[in] lastValue The last value to compare against.  Only used if \a writeToBitstream is true.
	/// \return true if \a writeToBitstream is true.  true if \a writeToBitstream is false and the read was successful.  false if \a writeToBitstream is false and the read was not successful.
	template <class templateType>
	bool SerializeCompressedDelta(bool writeToBitstream, templateType &currentValue, templateType lastValue);

	/// Save as SerializeCompressedDelta(templateType &currentValue, templateType lastValue) when we have an unknown second parameter
	template <class templateType>
	bool SerializeCompressedDelta(bool writeToBitstream, templateType &currentValue);

	/// Bidirectional serialize/deserialize an array or casted stream or raw data.  This does NOT do endian swapping.
	/// \param[in] writeToBitstream true to write from your data to this bitstream.  False to read from this bitstream and write to your data
	/// \param[in] input a byte buffer 
	/// \param[in] numberOfBytes the size of \a input in bytes 
	/// \return true if \a writeToBitstream is true.  true if \a writeToBitstream is false and the read was successful.  false if \a writeToBitstream is false and the read was not successful.
	bool Serialize(bool writeToBitstream, char* input, const int numberOfBytes);

	/// Bidirectional serialize/deserialize a normalized 3D vector, using (at most) 4 bytes + 3 bits instead of 12-24 bytes.  Will further compress y or z axis aligned vectors.
	/// Accurate to 1/32767.5.
	/// \param[in] writeToBitstream true to write from your data to this bitstream.  False to read from this bitstream and write to your data
	/// \param[in] x x
	/// \param[in] y y
	/// \param[in] z z
	/// \return true if \a writeToBitstream is true.  true if \a writeToBitstream is false and the read was successful.  false if \a writeToBitstream is false and the read was not successful.
	template <class templateType> // templateType for this function must be a float or double
	bool SerializeNormVector(bool writeToBitstream, templateType &x, templateType &y, templateType &z);

	/// Bidirectional serialize/deserialize a vector, using 10 bytes instead of 12.
	/// Loses accuracy to about 3/10ths and only saves 2 bytes, so only use if accuracy is not important.
	/// \param[in] writeToBitstream true to write from your data to this bitstream.  False to read from this bitstream and write to your data
	/// \param[in] x x
	/// \param[in] y y
	/// \param[in] z z
	/// \return true if \a writeToBitstream is true.  true if \a writeToBitstream is false and the read was successful.  false if \a writeToBitstream is false and the read was not successful.
	template <class templateType> // templateType for this function must be a float or double
	bool SerializeVector(bool writeToBitstream, templateType &x, templateType &y, templateType &z);

	/// Bidirectional serialize/deserialize a normalized quaternion in 6 bytes + 4 bits instead of 16 bytes.  Slightly lossy.
	/// \param[in] writeToBitstream true to write from your data to this bitstream.  False to read from this bitstream and write to your data
	/// \param[in] w w
	/// \param[in] x x
	/// \param[in] y y
	/// \param[in] z z
	/// \return true if \a writeToBitstream is true.  true if \a writeToBitstream is false and the read was successful.  false if \a writeToBitstream is false and the read was not successful.
	template <class templateType> // templateType for this function must be a float or double
	bool SerializeNormQuat(bool writeToBitstream, templateType &w, templateType &x, templateType &y, templateType &z);

	/// Bidirectional serialize/deserialize an orthogonal matrix by creating a quaternion, and writing 3 components of the quaternion in 2 bytes each
	/// for 6 bytes instead of 36
	/// Lossy, although the result is renormalized
	template <class templateType> // templateType for this function must be a float or double
	bool SerializeOrthMatrix(
		bool writeToBitstream,
		templateType &m00, templateType &m01, templateType &m02,
		templateType &m10, templateType &m11, templateType &m12,
		templateType &m20, templateType &m21, templateType &m22);

	/// Bidirectional serialize/deserialize numberToSerialize bits to/from the input. Right aligned
	/// data means in the case of a partial byte, the bits are aligned
	/// from the right (bit 0) rather than the left (as in the normal
	/// internal representation) You would set this to true when
	/// writing user data, and false when copying bitstream data, such
	/// as writing one bitstream to another
	/// \param[in] writeToBitstream true to write from your data to this bitstream.  False to read from this bitstream and write to your data
	/// \param[in] input The data 
	/// \param[in] numberOfBitsToSerialize The number of bits to write 
	/// \param[in] rightAlignedBits if true data will be right aligned 
	/// \return true if \a writeToBitstream is true.  true if \a writeToBitstream is false and the read was successful.  false if \a writeToBitstream is false and the read was not successful.
	bool SerializeBits(bool writeToBitstream, unsigned char* input, int numberOfBitsToSerialize, const bool rightAlignedBits = true);

	/// Write any integral type to a bitstream.  Undefine __BITSTREAM_NATIVE_END if you need endian swapping.
	/// \param[in] var The value to write
	template <class templateType>
	void Write(templateType var);

	/// Write any integral type to a bitstream.  If the current value is different from the last value
	/// the current value will be written.  Otherwise, a single bit will be written
	/// \param[in] currentValue The current value to write
	/// \param[in] lastValue The last value to compare against
	template <class templateType>
	void WriteDelta(templateType currentValue, templateType lastValue);

	/// WriteDelta when you don't know what the last value is, or there is no last value.
	/// \param[in] currentValue The current value to write
	template <class templateType>
	void WriteDelta(templateType currentValue);

	/// Write any integral type to a bitstream.  Undefine __BITSTREAM_NATIVE_END if you need endian swapping.
	/// If you are not using __BITSTREAM_NATIVE_END the opposite is true for types larger than 1 byte
	/// For floating point, this is lossy, using 2 bytes for a float and 4 for a double.  The range must be between -1 and +1.
	/// For non-floating point, this is lossless, but only has benefit if you use less than half the range of the type
	/// \param[in] var The value to write
	template <class templateType>
	void WriteCompressed(templateType var);

	/// Write any integral type to a bitstream.  If the current value is different from the last value
	/// the current value will be written.  Otherwise, a single bit will be written
	/// For floating point, this is lossy, using 2 bytes for a float and 4 for a double.  The range must be between -1 and +1.
	/// For non-floating point, this is lossless, but only has benefit if you use less than half the range of the type
	/// If you are not using __BITSTREAM_NATIVE_END the opposite is true for types larger than 1 byte
	/// \param[in] currentValue The current value to write
	/// \param[in] lastValue The last value to compare against
	template <class templateType>
	void WriteCompressedDelta(templateType currentValue, templateType lastValue);

	/// Save as WriteCompressedDelta(templateType currentValue, templateType lastValue) when we have an unknown second parameter
	template <class templateType>
	void WriteCompressedDelta(templateType currentValue);

	/// Read any integral type from a bitstream.  Define __BITSTREAM_NATIVE_END if you need endian swapping.
	/// \param[in] var The value to read
	template <class templateType>
	bool Read(templateType &var);

	/// Read any integral type from a bitstream.  If the written value differed from the value compared against in the write function,
	/// var will be updated.  Otherwise it will retain the current value.
	/// ReadDelta is only valid from a previous call to WriteDelta
	/// \param[in] var The value to read
	template <class templateType>
	bool ReadDelta(templateType &var);

	/// Read any integral type from a bitstream.  Undefine __BITSTREAM_NATIVE_END if you need endian swapping.
	/// For floating point, this is lossy, using 2 bytes for a float and 4 for a double.  The range must be between -1 and +1.
	/// For non-floating point, this is lossless, but only has benefit if you use less than half the range of the type
	/// If you are not using __BITSTREAM_NATIVE_END the opposite is true for types larger than 1 byte
	/// \param[in] var The value to read
	template <class templateType>
	bool ReadCompressed(templateType &var);

	/// Read any integral type from a bitstream.  If the written value differed from the value compared against in the write function,
	/// var will be updated.  Otherwise it will retain the current value.
	/// the current value will be updated.
	/// For floating point, this is lossy, using 2 bytes for a float and 4 for a double.  The range must be between -1 and +1.
	/// For non-floating point, this is lossless, but only has benefit if you use less than half the range of the type
	/// If you are not using __BITSTREAM_NATIVE_END the opposite is true for types larger than 1 byte
	/// ReadCompressedDelta is only valid from a previous call to WriteDelta
	/// \param[in] var The value to read
	template <class templateType>
	bool ReadCompressedDelta(templateType &var);

	/// Write an array or casted stream or raw data.  This does NOT do endian swapping.
	/// \param[in] input a byte buffer 
	/// \param[in] numberOfBytes the size of \a input in bytes 
	void Write(const char* input, const int numberOfBytes);

	/// Write one bitstream to another
	/// \param[in] numberOfBits bits to write
	/// \param bitStream the bitstream to copy from
	void Write(BitStream *bitStream, int numberOfBits);
	void Write(BitStream *bitStream);

	/// Read a normalized 3D vector, using (at most) 4 bytes + 3 bits instead of 12-24 bytes.  Will further compress y or z axis aligned vectors.
	/// Accurate to 1/32767.5.
	/// \param[in] x x
	/// \param[in] y y
	/// \param[in] z z
	template <class templateType> // templateType for this function must be a float or double
	void WriteNormVector(templateType x, templateType y, templateType z);

	/// Write a vector, using 10 bytes instead of 12.
	/// Loses accuracy to about 3/10ths and only saves 2 bytes, so only use if accuracy is not important.
	/// \param[in] x x
	/// \param[in] y y
	/// \param[in] z z
	template <class templateType> // templateType for this function must be a float or double
	void WriteVector(templateType x, templateType y, templateType z);

	/// Write a normalized quaternion in 6 bytes + 4 bits instead of 16 bytes.  Slightly lossy.
	/// \param[in] w w
	/// \param[in] x x
	/// \param[in] y y
	/// \param[in] z z
	template <class templateType> // templateType for this function must be a float or double
	void WriteNormQuat(templateType w, templateType x, templateType y, templateType z);

	/// Write an orthogonal matrix by creating a quaternion, and writing 3 components of the quaternion in 2 bytes each
	/// for 6 bytes instead of 36
	/// Lossy, although the result is renormalized
	template <class templateType> // templateType for this function must be a float or double
	void WriteOrthMatrix(
		templateType m00, templateType m01, templateType m02,
		templateType m10, templateType m11, templateType m12,
		templateType m20, templateType m21, templateType m22);

	/// Read an array or casted stream of byte. The array
	/// is raw data. There is no automatic endian conversion with this function
	/// \param[in] output The result byte array. It should be larger than @em numberOfBytes. 
	/// \param[in] numberOfBytes The number of byte to read
	/// \return true on success false if there is some missing bytes. 
	bool Read(char* output, const int numberOfBytes);

	///Sets the read pointer back to the beginning of your data.
	void ResetReadPointer(void);

	/// Sets the write pointer back to the beginning of your data.
	void ResetWritePointer(void);

	///This is good to call when you are done with the stream to make
	/// sure you didn't leave any data left over void
	void AssertStreamEmpty(void);

	/// Ignore data we don't intend to read
	/// \param[in] numberOfBits The number of bits to ignore
	void IgnoreBits(const int numberOfBits);

	///Move the write pointer to a position on the array.  
	/// \param[in] offset the offset from the start of the array. 
	/// \attention 
	/// Dangerous if you don't know what you are doing! 	
	/// For efficiency reasons you can only write mid-stream if your data is byte aligned.
	void SetWriteOffset(const int offset);

	/// Returns the length in bits of the stream
	inline int GetNumberOfBitsUsed(void) const { return GetWriteOffset(); }
	inline int GetWriteOffset(void) const { return numberOfBitsUsed; }

	///Returns the length in bytes of the stream
	inline int GetNumberOfBytesUsed(void) const { return BITS_TO_BYTES(numberOfBitsUsed); }

	///Returns the number of bits into the stream that we have read
	inline int GetReadOffset(void) const { return readOffset; }

	// Sets the read bit index
	inline void SetReadOffset(int newReadOffset) { readOffset = newReadOffset; }

	///Returns the number of bits left in the stream that haven't been read
	inline int GetNumberOfUnreadBits(void) const { return numberOfBitsUsed - readOffset; }

	/// Makes a copy of the internal data for you \a _data will point to
	/// the stream. Returns the length in bits of the stream. Partial
	/// bytes are left aligned 
	/// \param[out] _data The allocated copy of GetData()
	int CopyData(unsigned char** _data) const;

	/// Set the stream to some initial data.
	/// \internal
	void SetData(unsigned char *input);

	/// Gets the data that BitStream is writing to / reading from
	/// Partial bytes are left aligned.
	/// \return A pointer to the internal state 
	inline unsigned char* GetData(void) const { return data; }

	/// Write numberToWrite bits from the input source Right aligned
	/// data means in the case of a partial byte, the bits are aligned
	/// from the right (bit 0) rather than the left (as in the normal
	/// internal representation) You would set this to true when
	/// writing user data, and false when copying bitstream data, such
	/// as writing one bitstream to another
	/// \param[in] input The data 
	/// \param[in] numberOfBitsToWrite The number of bits to write 
	/// \param[in] rightAlignedBits if true data will be right aligned 
	void WriteBits(const unsigned char* input, int numberOfBitsToWrite, const bool rightAlignedBits = true);

	/// Align the bitstream to the byte boundary and then write the
	/// specified number of bits.  This is faster than WriteBits but
	/// wastes the bits to do the alignment and requires you to call
	/// ReadAlignedBits at the corresponding read position.
	/// \param[in] input The data
	/// \param[in] numberOfBytesToWrite The size of data. 
	void WriteAlignedBytes(const unsigned char *input, const int numberOfBytesToWrite);

	/// Read bits, starting at the next aligned bits. Note that the
	/// modulus 8 starting offset of the sequence must be the same as
	/// was used with WriteBits. This will be a problem with packet
	/// coalescence unless you byte align the coalesced packets.
	/// \param[in] output The byte array larger than @em numberOfBytesToRead
	/// \param[in] numberOfBytesToRead The number of byte to read from the internal state 
	/// \return true if there is enough byte. 
	bool ReadAlignedBytes(unsigned char *output, const int numberOfBytesToRead);

	/// Align the next write and/or read to a byte boundary.  This can
	/// be used to 'waste' bits to byte align for efficiency reasons It
	/// can also be used to force coalesced bitstreams to start on byte
	/// boundaries so so WriteAlignedBits and ReadAlignedBits both
	/// calculate the same offset when aligning.
	void AlignWriteToByteBoundary(void);

	/// Align the next write and/or read to a byte boundary.  This can
	/// be used to 'waste' bits to byte align for efficiency reasons It
	/// can also be used to force coalesced bitstreams to start on byte
	/// boundaries so so WriteAlignedBits and ReadAlignedBits both
	/// calculate the same offset when aligning.
	void AlignReadToByteBoundary(void);

	/// Read \a numberOfBitsToRead bits to the output source
	/// alignBitsToRight should be set to true to convert internal
	/// bitstream data to userdata. It should be false if you used
	/// WriteBits with rightAlignedBits false
	/// \param[in] output The resulting bits array 
	/// \param[in] numberOfBitsToRead The number of bits to read 
	/// \param[in] alignBitsToRight if true bits will be right aligned. 
	/// \return true if there is enough bits to read 
	bool ReadBits(unsigned char *output, int numberOfBitsToRead, const bool alignBitsToRight = true);

	/// Write a 0  
	void Write0(void);

	/// Write a 1 
	void Write1(void);

	/// Reads 1 bit and returns true if that bit is 1 and false if it is 0
	bool ReadBit(void);

	/// If we used the constructor version with copy data off, this
	/// *makes sure it is set to on and the data pointed to is copied.
	void AssertCopyData(void);

	/// Use this if you pass a pointer copy to the constructor
	/// *(_copyData==false) and want to overallocate to prevent
	/// *reallocation
	void SetNumberOfBitsAllocated(const unsigned int lengthInBits);

	/// Reallocates (if necessary) in preparation of writing numberOfBitsToWrite 
	void AddBitsAndReallocate(const int numberOfBitsToWrite);

private:
	/// Assume the input source points to a native type, compress and write it.
	void WriteCompressed(const unsigned char* input, const int size, const bool unsignedData);

	/// Assume the input source points to a compressed native type. Decompress and read it.
	bool ReadCompressed(unsigned char* output, const int size, const bool unsignedData);

	void ReverseBytes(unsigned char *input, unsigned char *output, int length);

	int numberOfBitsUsed;

	int numberOfBitsAllocated;

	int readOffset;

	unsigned char *data;

	/// true if the internal buffer is copy of the data passed to the constructor
	bool copyData;

	/// BitStreams that use less than BITSTREAM_STACK_ALLOCATION_SIZE use the stack, rather than the heap to store data.  It switches over if BITSTREAM_STACK_ALLOCATION_SIZE is exceeded
	unsigned char stackData[BITSTREAM_STACK_ALLOCATION_SIZE];
};

template <class templateType>
inline bool BitStream::Serialize(bool writeToBitstream, templateType &var)
{
	if (writeToBitstream)
		Write(var);
	else
		return Read(var);
	return true;
}

template <class templateType>
inline bool BitStream::SerializeDelta(bool writeToBitstream, templateType &currentValue, templateType lastValue)
{
	if (writeToBitstream)
		WriteDelta(currentValue, lastValue);
	else
		return ReadDelta(currentValue);
	return true;
}

template <class templateType>
inline bool BitStream::SerializeDelta(bool writeToBitstream, templateType &currentValue)
{
	if (writeToBitstream)
		WriteDelta(currentValue);
	else
		return ReadDelta(currentValue);
	return true;
}

template <class templateType>
inline bool BitStream::SerializeCompressed(bool writeToBitstream, templateType &var)
{
	if (writeToBitstream)
		WriteCompressed(var);
	else
		return ReadCompressed(var);
	return true;
}

template <class templateType>
inline bool BitStream::SerializeCompressedDelta(bool writeToBitstream, templateType &currentValue, templateType lastValue)
{
	if (writeToBitstream)
		WriteCompressedDelta(currentValue, lastValue);
	else
		return ReadCompressedDelta(currentValue);
	return true;
}

template <class templateType>
inline bool BitStream::SerializeCompressedDelta(bool writeToBitstream, templateType &currentValue)
{
	if (writeToBitstream)
		WriteCompressedDelta(currentValue);
	else
		return ReadCompressedDelta(currentValue);
	return true;
}

inline bool BitStream::Serialize(bool writeToBitstream, char* input, const int numberOfBytes)
{
	if (writeToBitstream)
		Write(input, numberOfBytes);
	else
		return Read(input, numberOfBytes);
	return true;
}

template <class templateType>
inline bool BitStream::SerializeNormVector(bool writeToBitstream, templateType &x, templateType &y, templateType &z)
{
	if (writeToBitstream)
		WriteNormVector(x, y, z);
	else
		return ReadNormVector(x, y, z);
	return true;
}

template <class templateType>
inline bool BitStream::SerializeVector(bool writeToBitstream, templateType &x, templateType &y, templateType &z)
{
	if (writeToBitstream)
		WriteVector(x, y, z);
	else
		return ReadVector(x, y, z);
	return true;
}

template <class templateType>
inline bool BitStream::SerializeNormQuat(bool writeToBitstream, templateType &w, templateType &x, templateType &y, templateType &z)
{
	if (writeToBitstream)
		WriteNormQuat(w, x, y, z);
	else
		return ReadNormQuat(w, x, y, z);
	return true;
}

template <class templateType>
inline bool BitStream::SerializeOrthMatrix(
	bool writeToBitstream,
	templateType &m00, templateType &m01, templateType &m02,
	templateType &m10, templateType &m11, templateType &m12,
	templateType &m20, templateType &m21, templateType &m22)
{
	if (writeToBitstream)
		WriteOrthMatrix(m00, m01, m02, m10, m11, m12, m20, m21, m22);
	else
		return ReadOrthMatrix(m00, m01, m02, m10, m11, m12, m20, m21, m22);
	return true;
}

inline bool BitStream::SerializeBits(bool writeToBitstream, unsigned char* input, int numberOfBitsToSerialize, const bool rightAlignedBits)
{
	if (writeToBitstream)
		WriteBits(input, numberOfBitsToSerialize, rightAlignedBits);
	else
		return ReadBits(input, numberOfBitsToSerialize, rightAlignedBits);
	return true;
}

template <class templateType>
inline void BitStream::Write(templateType var)
{
#ifdef _MSC_VER
#pragma warning(disable:4127)   // conditional expression is constant
#endif
	if (sizeof(var) == 1)
		WriteBits((unsigned char*)& var, sizeof(templateType) * 8, true);
	else
	{
		WriteBits((unsigned char*)& var, sizeof(templateType) * 8, true);
	}
}

/// Write a bool to a bitstream
/// \param[in] var The value to write
template <>
inline void BitStream::Write(bool var)
{
	if (var)
		Write1();
	else
		Write0();
}

/// Write any integral type to a bitstream.  Undefine __BITSTREAM_NATIVE_END if you need endian swapping.
/// For floating point, this is lossy, using 2 bytes for a float and 4 for a double.  The range must be between -1 and +1.
/// For non-floating point, this is lossless, but only has benefit if you use less than half the range of the type
/// If you are not using __BITSTREAM_NATIVE_END the opposite is true for types larger than 1 byte
/// \param[in] var The value to write
template <class templateType>
inline void BitStream::WriteCompressed(templateType var)
{
#ifdef _MSC_VER
#pragma warning(disable:4127)   // conditional expression is constant
#endif
	if (sizeof(var) == 1)
		WriteCompressed((unsigned char*)& var, sizeof(templateType) * 8, true);
	else
	{
		WriteCompressed((unsigned char*)& var, sizeof(templateType) * 8, true);
	}
}

template <>
inline void BitStream::WriteCompressed(bool var)
{
	Write(var);
}

/// For values between -1 and 1
template <>
inline void BitStream::WriteCompressed(float var)
{
	assert(var > -1.01f && var < 1.01f);
	if (var < -1.0f)
		var = -1.0f;
	if (var > 1.0f)
		var = 1.0f;
	Write((unsigned short)((var + 1.0f)*32767.5f));
}

/// For values between -1 and 1
template <>
inline void BitStream::WriteCompressed(double var)
{
	assert(var > -1.01 && var < 1.01);
	if (var < -1.0f)
		var = -1.0f;
	if (var > 1.0f)
		var = 1.0f;
#ifdef _DEBUG
	assert(sizeof(unsigned long) == 4);
#endif
	Write((unsigned long)((var + 1.0)*2147483648.0));
}

/// Write any integral type to a bitstream.  If the current value is different from the last value
/// the current value will be written.  Otherwise, a single bit will be written
/// For floating point, this is lossy, using 2 bytes for a float and 4 for a double.  The range must be between -1 and +1.
/// For non-floating point, this is lossless, but only has benefit if you use less than half the range of the type
/// If you are not using __BITSTREAM_NATIVE_END the opposite is true for types larger than 1 byte
/// \param[in] currentValue The current value to write
/// \param[in] lastValue The last value to compare against
template <class templateType>
inline void BitStream::WriteCompressedDelta(templateType currentValue, templateType lastValue)
{
	if (currentValue == lastValue)
	{
		Write(false);
	}
	else
	{
		Write(true);
		WriteCompressed(currentValue);
	}
}

/// Write a bool delta.  Same thing as just calling Write
/// \param[in] currentValue The current value to write
/// \param[in] lastValue The last value to compare against
template <>
inline void BitStream::WriteCompressedDelta(bool currentValue, bool lastValue)
{
#ifdef _MSC_VER
#pragma warning(disable:4100)   // warning C4100: 'lastValue' : unreferenced formal parameter
#endif
	Write(currentValue);
}

/// Save as WriteCompressedDelta(templateType currentValue, templateType lastValue) when we have an unknown second parameter
template <class templateType>
inline void BitStream::WriteCompressedDelta(templateType currentValue)
{
	Write(true);
	WriteCompressed(currentValue);
}

/// Save as WriteCompressedDelta(bool currentValue, templateType lastValue) when we have an unknown second bool
template <>
inline void BitStream::WriteCompressedDelta(bool currentValue)
{
	Write(currentValue);
}

/// Read any integral type from a bitstream.  Define __BITSTREAM_NATIVE_END if you need endian swapping.
/// \param[in] var The value to read
template <class templateType>
inline bool BitStream::Read(templateType &var)
{
#ifdef _MSC_VER
#pragma warning(disable:4127)   // conditional expression is constant
#endif
	if (sizeof(var) == 1)
		return ReadBits((unsigned char*)&var, sizeof(templateType) * 8, true);
	else
	{
		return ReadBits((unsigned char*)& var, sizeof(templateType) * 8, true);
	}
}

/// Read a bool from a bitstream
/// \param[in] var The value to read
template <>
inline bool BitStream::Read(bool &var)
{
	if (readOffset + 1 > numberOfBitsUsed)
		return false;

	if (data[readOffset >> 3] & (0x80 >> (readOffset++ % 8)))   // Is it faster to just write it out here?
		var = true;
	else
		var = false;

	return true;
}

/// Read any integral type from a bitstream.  If the written value differed from the value compared against in the write function,
/// var will be updated.  Otherwise it will retain the current value.
/// ReadDelta is only valid from a previous call to WriteDelta
/// \param[in] var The value to read
template <class templateType>
inline bool BitStream::ReadDelta(templateType &var)
{
	bool dataWritten;
	bool success;
	success = Read(dataWritten);
	if (dataWritten)
		success = Read(var);
	return success;
}

/// Read a bool from a bitstream
/// \param[in] var The value to read
template <>
inline bool BitStream::ReadDelta(bool &var)
{
	return Read(var);
}

/// Read any integral type from a bitstream.  Undefine __BITSTREAM_NATIVE_END if you need endian swapping.
/// For floating point, this is lossy, using 2 bytes for a float and 4 for a double.  The range must be between -1 and +1.
/// For non-floating point, this is lossless, but only has benefit if you use less than half the range of the type
/// If you are not using __BITSTREAM_NATIVE_END the opposite is true for types larger than 1 byte
/// \param[in] var The value to read
template <class templateType>
inline bool BitStream::ReadCompressed(templateType &var)
{
#ifdef _MSC_VER
#pragma warning(disable:4127)   // conditional expression is constant
#endif
	if (sizeof(var) == 1)
		return ReadCompressed((unsigned char*)&var, sizeof(templateType) * 8, true);
	else
	{
		return ReadCompressed((unsigned char*)& var, sizeof(templateType) * 8, true);
	}
}

template <>
inline bool BitStream::ReadCompressed(bool &var)
{
	return Read(var);
}

/// For values between -1 and 1
template <>
inline bool BitStream::ReadCompressed(float &var)
{
	unsigned short compressedFloat;
	if (Read(compressedFloat))
	{
		var = ((float)compressedFloat / 32767.5f - 1.0f);
		return true;
	}
	return false;
}

/// For values between -1 and 1
template <>
inline bool BitStream::ReadCompressed(double &var)
{
	unsigned long compressedFloat;
	if (Read(compressedFloat))
	{
		var = ((double)compressedFloat / 2147483648.0 - 1.0);
		return true;
	}
	return false;
}


/// Read any integral type from a bitstream.  If the written value differed from the value compared against in the write function,
/// var will be updated.  Otherwise it will retain the current value.
/// the current value will be updated.
/// For floating point, this is lossy, using 2 bytes for a float and 4 for a double.  The range must be between -1 and +1.
/// For non-floating point, this is lossless, but only has benefit if you use less than half the range of the type
/// If you are not using __BITSTREAM_NATIVE_END the opposite is true for types larger than 1 byte
/// ReadCompressedDelta is only valid from a previous call to WriteDelta
/// \param[in] var The value to read
template <class templateType>
inline bool BitStream::ReadCompressedDelta(templateType &var)
{
	bool dataWritten;
	bool success;
	success = Read(dataWritten);
	if (dataWritten)
		success = ReadCompressed(var);
	return success;
}

/// Read a bool from a bitstream
/// \param[in] var The value to read
template <>
inline bool BitStream::ReadCompressedDelta(bool &var)
{
	return Read(var);
}