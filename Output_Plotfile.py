# ------- file: myplot.py ------
import matplotlib as mpl
import matplotlib.pyplot as plt
import numpy as np

 		
Epochs = 16
RMSE =  [2.52628, 2.31423, 2.30067 , 2.61843, 2.7911, 2.73312, 2.35191, 2.2396, 2.29488,  2.54868, 2.75801, 2.64818, 2.30436,  2.22717, 2.29739,  2.35195]
Kd = [1.51016,1.53003, 1.55462,  1.56843, 1.59489, 1.60498, 1.62704, 1.64781, 1.66778, 1.68055, 1.70681, 1.71706, 1.7387, 1.75906, 1.77685, 1.78846]
Ki = [0.000502281, 0.000490964, 0.000490478 , 0.000497808, 0.000521547, 0.000532048 , 0.000489763, 0.000435951, 0.00043012 , 0.000439878, 0.000473863, 0.000480691,0.000395837, 0.000316917, 0.000304281,  0.000297158]
Kp =[0.100592 , 0.101018 , 0.101857, 0.102422, 0.10365, 0.104094, 0.104392, 0.104826 , 0.105485, 0.105979, 0.107189, 0.10762 , 0.107883, 0.108301, 0.108874, 0.109262]

x =  list(range(0, Epochs))
print("x Array", x)
#ifg = plt.figure()

#plt.plot(x, RMSE,'-',  color='black')
#plt.plot(x, Kd, '--',  color='blue', label='Kd' );
#plt.plot(x, Kp, '--',   color='red', label='Kp' );
#plt.plot(x, Ki, '--',  color='green' , label='Ki' );

#plt.title('Training Progress', fontsize=25)
#plt.xlabel('Track Rounds', fontsize=20)
#plt.ylabel('non', fontsize=20)

#plt.show()  # Output 


# Set parameter for Plot
xticks = [0, 5,10, 15]
#ax.set_xticklabels(["$%.1f$" % y for y in yticks], fontsize=18); # use LaTeX formatted labels

fig, (ax1, ax2, ax3 ) = plt.subplots(1,3)

ax1.title.set_text('RMSE')
ax1.set_xlabel('Track Round')
ax1.set_ylabel('Error', color='black')
ax1.plot(x, RMSE,'-',  color='black')
ax1.set_xticks(xticks)
#ax1.tick_params(axis='y', labelcolor='black')
#ax1.legend(['RMSE'])

ax2.title.set_text('Kd' )
ax2.set_xlabel('Track Round')
ax2.set_ylabel('Value', color='black')
ax2.plot(x, Kd, '--',  color='blue', label='Kd' );
ax2.set_xticks(xticks)
#ax2.tick_params(axis='y', labelcolor='black')
#ax2.title('Kd', fontsize=25)

ax3.title.set_text('Kp & Ki')
ax3.set_xlabel('Track Round')
ax3.set_ylabel('Value', color='black')  
ax3.plot(x, Kp, '--',   color='red', label='Kp' );
ax3.plot(x, Ki, '--',  color='green' , label='Ki' );
ax3.set_xticks(xticks)
#ax3.tick_params(axis='y', labelcolor='black')
sax3.legend(['Kp', 'Ki'])

fig.tight_layout()

#fig.title('Training Progress', fontsize=25)
#plt.xlabel('Track Rounds', fontsize=20)
#plt.ylabel(''', fontsize=20)
 
plt.show()  # Output
 
 
 
