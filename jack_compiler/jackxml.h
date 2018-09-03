#ifndef JACKXML_H
#define JACKXML_H

#include <string>

// the jackxml class writes XML nodes and data to cout
// element names must only include, letters, digits, '-' '_' and '.'
// it will translate <,>,&,'," to HTML entities
// all output is kept in a buffer until flush_output() is called
// arbitrary lines of text can also be buffered using buffer_line()
// if necessary, all current buffered output can be discarded
class jackxml
{
public:
	static void open_node(std::string element) ;	// buffers <elementname>
	static void node_text(std::string content) ;	// buffers the content but translates &,<,>,',"
	static void close_node(std::string element) ;	// buffers </elementname>
	static void buffer_line(std::string line) ;	// buffers line of data for output
	static void erase_buffer() ;			// erase the buffer, nothing is output
	static void flush_output() ;			// output buffer contents, adding endl to each line, and empty the buffer
};
#endif //JACKXML_H
