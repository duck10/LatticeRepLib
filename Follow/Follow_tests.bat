Follow
random 15
points 400
enabke ri
enble v7
enable u
disble nc
disable cs
mode splyne
p 15 15 15 090 90 090
random 8
p 10 10 10 120 120 120
showmarkers true
end

Follow
random 12
points 600
enable ri
enable v7
perturbations 5
perturbby 0.15
mode spline
p 12 12 12 075 075 075
random 4
p 8 8 8 100 100 100
glitchthreshold 3.5
end

Follow
random 8
points 1000
enable u
;disable nc
;disable cs
mode point
p 20 20 20 080 80 080
random 10
p 15 15 15 110 110 110
blocksize 500
blockstart 250
end

Follow
random 20
points 300
enable ri
enabke v7
enable cs
printdata true
glitchesonly true
mode spline
p 25 25 25 070 070 070
random 5
p 18 18 18 095 95 095
end

Follow
random 10
points 200
mode point
p 10 10 10 60 60 60
end

Follow
random 10
points 200
mode line
p 10 10 10 60 60 60
p 20 20 20 70 70 70
end

Follow
random 10
points 200
mode chord
p 10 10 10 60 60 60
p 20 20 20 70 70 70
end

Follow
random 10
points 200
mode chord3
p 10 10 10 60 60 60
p 20 20 20 70 70 70
p 30 30 30 80 80 80
end

Follow
random 10
points 200
mode triangle
p 10 10 10 60 60 60
p 20 20 20 70 70 70
p 30 30 30 80 80 80
end

Follow
random 10
points 200
mode spline
p 10 10 10 60 60 60
p 20 20 20 70 70 70
end

Follow
random 10
points 200
mode triangle
p 10 10 10 60 60 60
p 20 20 20 70 70 70
; this should fail - not enough vectors for triangle mode
end

