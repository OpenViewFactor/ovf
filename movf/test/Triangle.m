classdef Triangle < handle
  properties (GetAccess = private, SetAccess = private)
    OA (1,1) Vector3 = Vector3(0,0,0)
    OB (1,1) Vector3 = Vector3(0,0,0)
    OC (1,1) Vector3 = Vector3(0,0,0)
    NORMAL (1,1) Vector3 = Vector3(0,0,0)
    AREA (1,1) double = 0
  end

  methods
    function obj = Triangle(OA, OB, OC)
      arguments
        OA (1,1) Vector3 = Vector3(0,0,0)
        OB (1,1) Vector3 = Vector3(0,0,0)
        OC (1,1) Vector3 = Vector3(0,0,0)
      end
      obj.OA = OA;
      obj.OB = OB;
      obj.OC = OC;
      obj.NORMAL = obj.getAB.crossProduct(obj.getCA.flip);
      obj.AREA = obj.NORMAL.getMagnitude / 2;
    end

    function OA = getOA(obj)
      OA = obj.OA;
    end
    function OB = getOB(obj)
      OB = obj.OB;
    end
    function OC = getOC(obj)
      OC = obj.OC;
    end

    function AB = getAB(obj)
      AB = obj.getOB - obj.getOA;
    end
    function BC = getBC(obj)
      BC = obj.getOC - obj.getOB;
    end
    function CA = getCA(obj)
      CA = obj.getOA - obj.getOC;
    end

    function N = normal(obj)
      N = obj.NORMAL;
    end
    function C = centroid(obj)
      v = obj.getOA + obj.getOB + obj.getOC;
      C = Vector3(v.getX / 3, v.getY / 3, v.getZ / 3);
    end
    function A = area(obj)
      A = obj.AREA;
    end

    function setOA(obj, x, y, z)
      arguments
        obj Triangle
        x (1,1) double
        y (1,1) double
        z (1,1) double
      end
      obj.OA = Vector3(x, y, z);
    end
    function setOB(obj, x, y, z)
      arguments
        obj Triangle
        x (1,1) double
        y (1,1) double
        z (1,1) double
      end
      obj.OB = Vector3(x, y, z);
    end
    function setOC(obj, x, y, z)
      arguments
        obj Triangle
        x (1,1) double
        y (1,1) double
        z (1,1) double
      end
      obj.OC = Vector3(x, y, z);
    end

    function tf = eq(obj, T)
      arguments
        obj Triangle
        T Triangle
      end
      tf = obj.getOA == T.getOA && obj.getOB == T.getOB && obj.getOC == T.getOC;
    end
  end
end