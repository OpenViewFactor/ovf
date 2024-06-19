classdef Vector3 < handle
  properties (GetAccess = private, SetAccess = private)
    x (1,1) double = 0
    y (1,1) double = 0
    z (1,1) double = 0
  end

  methods
    function obj = Vector3(x, y, z)
      arguments
        x (1,1) double
        y (1,1) double
        z (1,1) double
      end
      obj.x = x;
      obj.y = y;
      obj.z = z;
    end

    function x = getX(obj)
      x = obj.x;
    end
    function y = getY(obj)
      y = obj.y;
    end
    function z = getZ(obj)
      z = obj.z;
    end
    function val = getComponentByAxis(obj, axis)
      arguments
        obj Vector3
        axis (1,1) uint64
      end
      switch axis
        case 1
          val = obj.getX;
        case 2
          val = obj.getY;
        case 3
          val = obj.getZ;
      end
    end

    function axis_index = getLongestDirection(obj)
      axis_index = 1; % default axis to split to x
      if obj.getY > obj.getX && obj.getY > obj.getZ
        axis_index = 2; % set axis to split to y
      elseif obj.getZ > obj.getX && obj.getZ > obj.getY
        axis_index = 3;
      end
    end

    function magnitude = getMagnitude(obj)
      magnitude = norm([obj.getX, obj.getY, obj.getZ]);
    end

    function setX(obj, x)
      arguments
        obj Vector3
        x (1,1) double
      end
      obj.x = x;
    end
    function setY(obj, y)
      arguments
        obj Vector3
        y (1,1) double
      end
      obj.y = y;
    end
    function setZ(obj, z)
      arguments
        obj Vector3
        z (1,1) double
      end
      obj.z = z;
    end

    function dot_product = dotProduct(obj, v)
      arguments
        obj Vector3
        v Vector3
      end
      dot_product = (obj.getX * v.getX + obj.getY * v.getY + obj.getZ * v.getZ);
    end
    function cross_product = crossProduct(obj, v)
      arguments
        obj Vector3
        v Vector3
      end
      cross_x = obj.getY * v.getZ - obj.getZ * v.getY;
      cross_y = -1 * (obj.getX * v.getZ - obj.getZ * v.getX);
      cross_z = obj.getX * v.getY - obj.getY * v.getX;
      cross_product = Vector3(cross_x, cross_y, cross_z);
    end

    function dir = direction(obj)
      dir = Vector3(obj.getX / obj.getMagnitude, obj.getY / obj.getMagnitude, obj.getZ / obj.getMagnitude);
    end

    function flipped_vector = flip(obj)
      flipped_vector = Vector3(obj.getX * (-1), obj.getY * (-1), obj.getZ * (-1));
    end

    function sum = plus(obj, v)
      arguments
        obj Vector3
        v Vector3
      end
      sum = Vector3(obj.getX + v.getX, obj.getY + v.getY, obj.getZ + v.getZ);
    end
    function diff = minus(obj, v)
      arguments
        obj Vector3
        v Vector3
      end
      diff = Vector3(obj.getX - v.getX, obj.getY - v.getY, obj.getZ - v.getZ);
    end

    function tf = eq(obj, v)
      arguments
        obj Vector3
        v Vector3
      end
      tf = obj.getX == v.getX && obj.getY == v.getY && obj.getZ == v.getZ;
    end
  end
end