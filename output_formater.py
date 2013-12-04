##
## Formats certain output types, namely tables and long strings.
##


def print_table( rows, column_names=[], row_names=[] ):
    """ Prints a table with column and row names.
    """
    num_rows=len(rows)
    if num_rows<0: return

    from collections import namedtuple
    def pprinttable(rows):
        headers = rows[0]._fields
        lens = []
        ls = lambda x:len(repr(x))
        for i in range(len(rows[0])):
            lens.append( 
                ls(max([x[i] for x in rows] + [headers[i]], key=ls)) )
        formats = []
        hformats = []
        for i in range(len(rows[0])):
            if isinstance(rows[0][i], int):
                formats.append("%%%dd" % lens[i])
            else:
                formats.append("%%-%ds" % lens[i])
            hformats.append("%%-%ds" % lens[i])
        pattern = " | ".join(formats)
        hpattern = " | ".join(hformats)
        separator = "-+-".join(['-' * n for n in lens])
        print hpattern % tuple(headers)
        print separator
        for line in rows:
            print pattern % tuple(line)

    # Append each element from row_names to the front of each row in rows.
    if len(row_names) == num_rows:
        newrows = [ [h]+row for (h,row) in zip( row_names, rows ) ]
        if len(column_names)+1 == len(rows[0]):
            column_names=['']+column_names
    if len(column_names) < len(rows[0]):
        n = len(rows[0])-len(column_names)
        column_names+=""*n
    # Make each row a named tuple.
    r = namedtuple('r',column_names)
    newrows = rows
    pprinttable([ r(*row) for row in newrows ])


def print_longstring( row_size, num_row, s ):
    fin,i,out,l = False,0,"",len(s)
    for _ in range(num_row):
        n = min(i+row_size, l)
        out+=s[i:n]+"\n"
        if n==l:
            fin=True
            break
    if not fin and n>3:
        out=out[:-4]+"...\n"
    print out,
