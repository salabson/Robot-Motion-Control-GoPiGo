;; Auto-generated. Do not edit!


(when (boundp 'gopigo_interface::Encoder)
  (if (not (find-package "GOPIGO_INTERFACE"))
    (make-package "GOPIGO_INTERFACE"))
  (shadow 'Encoder (find-package "GOPIGO_INTERFACE")))
(unless (find-package "GOPIGO_INTERFACE::ENCODER")
  (make-package "GOPIGO_INTERFACE::ENCODER"))

(in-package "ROS")
;;//! \htmlinclude Encoder.msg.html


(defclass gopigo_interface::Encoder
  :super ros::object
  :slots (_right _left ))

(defmethod gopigo_interface::Encoder
  (:init
   (&key
    ((:right __right) 0.0)
    ((:left __left) 0.0)
    )
   (send-super :init)
   (setq _right (float __right))
   (setq _left (float __left))
   self)
  (:right
   (&optional __right)
   (if __right (setq _right __right)) _right)
  (:left
   (&optional __left)
   (if __left (setq _left __left)) _left)
  (:serialization-length
   ()
   (+
    ;; float32 _right
    4
    ;; float32 _left
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float32 _right
       (sys::poke _right (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _left
       (sys::poke _left (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float32 _right
     (setq _right (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _left
     (setq _left (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get gopigo_interface::Encoder :md5sum-) "5d581e8ae3609f50d2d05ff345e71418")
(setf (get gopigo_interface::Encoder :datatype-) "gopigo_interface/Encoder")
(setf (get gopigo_interface::Encoder :definition-)
      "float32 right
float32 left


")



(provide :gopigo_interface/Encoder "5d581e8ae3609f50d2d05ff345e71418")


