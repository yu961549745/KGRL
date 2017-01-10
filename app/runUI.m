clc,clear,close all;
f=ui();
hs=guihandles(f);
global jlog tasks
jlog=java.awt.TextArea();
jlog.setEditable(false);
jlog.setBackground(java.awt.Color.WHITE);
hlog=javacomponent(jlog,getpixelposition(hs.log),hs.frame);
tasks.FB_A_Train=@()mexKGRL(56,54,0.2228,217,5,'FB_A','LP_Train',300);
tasks.FB_A_Test=@()mexKGRL(56,54,0.2228,217,5,'FB_A','LP_Test',300);
tasks.FB_B_Train=@()mexKGRL(39,46,0.07,114,4,'FB_B','TC_Train',300);
tasks.FB_B_Test=@()mexKGRL(39,46,0.07,114,4,'FB_B','TC_Test',300);
tasks.WN_A_Train=@()mexKGRL(41,56,0.26,242,10,'WN_A','LP_Train',300);
tasks.WN_A_Test=@()mexKGRL(41,56,0.26,242,10,'WN_A','LP_Test',300);
tasks.WN_B_Train=@()mexKGRL(89,20,0.35,126,10,'WN_B','TC_Train',300);
tasks.WN_B_Test=@()mexKGRL(89,20,0.35,126,10,'WN_B','TC_Test',300);