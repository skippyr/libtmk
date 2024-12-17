#if !defined(_tmk_H)
#define _tmk_H

enum tmk_Stream {
	tmk_Stream_Input,
	tmk_Stream_Output,
	tmk_Stream_Error
};

#if defined(__cplusplus)
extern "C" {
#endif
int tmk_isStreamRedirected(int stream);
#if defined(__cplusplus)
}
#endif
#endif
