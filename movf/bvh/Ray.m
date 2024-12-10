classdef Ray < handle
  properties
    origin Vector3 = Vector3(0,0,0)
    direction Vector3 = Vector3(1,0,0)
    intersection_distance (1,1) double = inf
  end

  methods
    function obj = Ray(~)
    end

    function origin = getOrigin(obj)
      origin = obj.origin;
    end
    function direction = getDirection(obj)
      direction = obj.direction;
    end
    function intersection_distance = getIntersectionDistance(obj)
      intersection_distance = obj.intersection_distance;
    end
    function setOrigin(obj, origin)
      arguments
        obj Ray
        origin Vector3
      end
      obj.origin = origin;
    end
    function setDirection(obj, direction)
      arguments
        obj Ray
        direction Vector3
      end
      obj.direction = direction.direction;
    end
    function setIntersectionDistance(obj, distance)
      arguments
        obj Ray
        distance (1,1) double
      end
      obj.intersection_distance = distance;
    end

    function is_culled = backFaceCull(obj, emitter, receiver)
      arguments
        obj Ray
        emitter Triangle
        receiver Triangle
      end
      is_culled = obj.getDirection.dotProduct(emitter.getNormal) < 0 || obj.getDirection.dotProduct(receiver.getNormal) > 0;
    end
    function triangleIntersection(obj, triangle)
      arguments
        obj Ray
        triangle Triangle
      end
      V0 = triangle.getOA;
      E1 = triangle.getAB;
      E2 = triangle.getCA.flip;
    
      T = obj.getOrigin - V0;
      D = obj.getDirection;
      P = D.crossProduct(E2);
      Q = T.crossProduct(E1);
      
      t = Q.dotProduct(E2) * (1 / P.dotProduct(E1));
      if t > 0 && t < obj.getIntersectionDistance
        obj.setIntersectionDistance(t);
      end
    end

    function plotRay(obj)
      if obj.getIntersectionDistance ~= inf
        origin_point = [obj.getOrigin.getX, obj.getOrigin.getY, obj.getOrigin.getZ];
        end_point = origin_point + obj.getIntersectionDistance * [obj.getDirection.getX, obj.getDirection.getY, obj.getDirection.getZ];
        points = [origin_point; end_point];
        plot3(points(:,1), points(:,2), points(:,3), '-o', 'Color', 'b' , 'MarkerSize', 10, 'MarkerFaceColor', '#D9FFFF')
      end
    end
  end
end