class Record:
    def __init__(self, lines):
        pass

    def separateRecords(fname):
        records = []
        with open(fname,'r') as f:
            alines = None
            for line in f:
                lowl= line.rstrip().lower();
                if lowl.endswith(".jpg") or lowl.endswith(".jpeg") or lowl.endswith("png") or lowl.endswith("gif"):
                    if alines!=None:
                        records.append(Record(alines))
                    alines=[]
                alines.append(line)
            records.append(alines)

        print(len(records))


class Rect:
    def __init__(self, ax, ay, aw, ah, atype, aprop):
        self.x = ax
        self.y = ay
        self.w = aw
        self.h = ah
        self.t = atype
        self.prop = aprop

    def include(self,other):
        pass

    def to_s(self):
        pass

    def diff(self, other):
        pass

    def __add__(self, other):
        pass

    def __minus__(self, other):
        pass

    def __mul__(self, other):
        pass

    def __div__(self, other):
        pass


class Transform:
    def __init__(self,ax,ay,ar,vx,vy,vr)
    pass


