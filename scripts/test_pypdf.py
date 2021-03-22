import sys
from PyPDF2 import PdfFileReader

if __name__ == '__main__':
    # print(extract_text_from_pdf('cbcng2008.pdf'))
    # conn = sqlite3.connect('airway.db')

    if len(sys.argv) > 1:
        file = sys.argv[1]
        with open(file, "rb") as filehandle:
            pdf = PdfFileReader(filehandle)
            info = pdf.getDocumentInfo()
            pages = pdf.getNumPages()   
            # print (info)
            print("number of pages: %i" % pages)   
            page1 = pdf.getPage(0)
            print(page1)
            print('>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>')
            print(page1.extractText())
            page1 = pdf.getPage(1)
            print(page1)
            print('>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>')
            print(page1.extractText())