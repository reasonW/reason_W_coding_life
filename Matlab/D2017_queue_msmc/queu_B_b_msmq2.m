%% ���B ������������
 % state_a_b=randsrc(1,47,b_b_ser_time1);
% state_b_b=randsrc(1,47,b_b_ser_time2);
state_a_b=abs(exprnd(7.542,1,arr_num));
state_b_b=abs(exprnd(7.542,1,arr_num));

%st=exprnd(2.5,1,n); %����̨����ʱ��
bb1_a=zeros(1,arr_num); %ÿ����������ʱ��
bb2_a=zeros(1,arr_num); %ÿ��������ʼ���ܷ���ʱ��   -> events(3,:)
bb3_a=zeros(1,arr_num);%ÿ�������뿪ʱ��            -> events(4,:)
bb4_a=zeros(1,arr_num);% %��ʶλ��ʾ�����ϵͳ��ϵͳ�ڹ��еĹ˿���       -> events(5,:)
bb5_a=zeros(1,arr_num);% ��ʶλ�Ƿ���븴��
aerfa=0.02;
bb1_b=zeros(1,arr_num); %ÿ����������ʱ��
bb2_b=zeros(1,arr_num); %ÿ��������ʼ���ܷ���ʱ��   -> events(3,:)
bb3_b=zeros(1,arr_num);%ÿ�������뿪ʱ��            -> events(4,:)
bb4_b=zeros(1,arr_num);% %��ʶλ��ʾ�����ϵͳ��ϵͳ�ڹ��еĹ˿���       -> events(5,:)
bb5_b=zeros(1,arr_num);% ��ʶλ�Ƿ���븴��

bb1_a(1)=0;
% for i=2:arr_num
%   a1(i)=a1(i-1)+dt(i-1);%��i����������ʱ��  -> events(1,:)
% end
arr_time_b=a3;
bb1_a(1)=arr_time_b(1);%��1����������ʱ��
bb1_b(1)=arr_time_b(2);%��1����������ʱ��
bb2_a(1)=arr_time_b(1);%��1��������ʼ���ܷ���ʱ��Ϊ����ʱ��
bb2_b(1)=arr_time_b(2);%��1��������ʼ���ܷ���ʱ��Ϊ����ʱ��
bb3_a(1)=bb2_a(1)+state_a_b(1); %��1�������뿪ʱ��
bb3_b(1)=bb2_b(1)+state_b_b(1); %��1�������뿪ʱ��
bb4_a(1)=1;%һ���˿�,��־λ��Ϊ1
bb4_b(1)=1;%һ���˿�,��־λ��Ϊ1
bb5_a(1)=randsrc(1,1,[0 1;aerfa 1-aerfa]);
bb5_b(1)=randsrc(1,1,[0 1;aerfa 1-aerfa]);

%�����ϵͳ��ϵͳ�����г�Ա���Ϊ 1
member_A_b_b = [1];
member_B_b_b = [1];
bottleBb=zeros(1,10);
bottleBb(1)=0;
bottleBb(2)=0;
DeteBb=0;
DeteBb_t=[];
Pointer=2;

for i=3:arr_num
    if bb4_a(length(member_A_b_b))<=bb4_b(length(member_B_b_b))
        member_A_b_b=[member_A_b_b,length(member_A_b_b)+1];
        len_A_mem_b_b=length(member_A_b_b);
        bb1_a(len_A_mem_b_b)=arr_time_b(i);
        number=sum(bb3_a(1:len_A_mem_b_b-1)>=bb1_a(len_A_mem_b_b));
          if number ==0
            bb2_a(len_A_mem_b_b)=bb1_a(len_A_mem_b_b);
            bb3_a(len_A_mem_b_b)=bb1_a(len_A_mem_b_b)+state_a_b(len_A_mem_b_b);
            bb4_a(len_A_mem_b_b)=1;
            bb5_a(len_A_mem_b_b)=randsrc(1,1,[0 1;1-aerfa aerfa]);
          else
            bb2_a(len_A_mem_b_b)=bb3_a(len_A_mem_b_b-1);
            bb3_a(len_A_mem_b_b)=bb2_a(len_A_mem_b_b)+state_a_b(len_A_mem_b_b);
            bb4_a(len_A_mem_b_b)=number+1;
            bb5_a(len_A_mem_b_b)=randsrc(1,1,[0 1;1-aerfa aerfa]);
          end
          bottleBb(rem(Pointer+1,10)+1)=bb2_a(len_A_mem_b_b)-bb1_a(len_A_mem_b_b);
          Pointer=Pointer+1;
    else
         member_B_b_b=[member_B_b_b,length(member_B_b_b)+1];
         len_B_mem_b_b=length(member_B_b_b);
         bb1_b(len_B_mem_b_b)=arr_time_b(i);
         number=sum(bb3_b(1:len_B_mem_b_b-1)>=bb1_b(len_B_mem_b_b));
          if number ==0
            bb2_b(len_B_mem_b_b)=bb1_b(len_B_mem_b_b);
            bb3_b(len_B_mem_b_b)=bb1_b(len_B_mem_b_b)+state_b_b(len_B_mem_b_b);
            bb4_b(len_B_mem_b_b)=1;
            bb5_b(len_B_mem_b_b)=randsrc(1,1,[0 1;1-aerfa aerfa]);
          else
            bb2_b(len_B_mem_b_b)=bb3_b(len_B_mem_b_b-1);
            bb3_b(len_B_mem_b_b)=bb2_b(len_B_mem_b_b)+state_b_b(len_B_mem_b_b);
            bb4_b(len_B_mem_b_b)=number+1;
            bb5_b(len_B_mem_b_b)=randsrc(1,1,[0 1;1-aerfa aerfa]);
          end
          bottleBb(rem(Pointer+1,10)+1)=bb2_b(len_B_mem_b_b)-bb1_b(len_B_mem_b_b);
          Pointer=Pointer+1;
    end
    if (MODEL==1)
        if sum(bottleBb)/10>6.31
%          fprintf('B bag area %f people waiting long %f \n',i,sum(bottleA)/20);
            DeteBb=DeteBb+1;
            DeteBb_t=[DeteBb_t,sum(bottleBb)/10];
        end
    else
        if sum(bottleBb)/10>8.11
%            fprintf('B bag area %f people waiting long %f \n',i,sum(bottleA)/20);
           DeteBb=DeteBb+1;
           DeteBb_t=[DeteBb_t,sum(bottleBb)/10];
        end
    end
end
%% demo_��ͼ
len_A_mem_b_b = length(member_A_b_b);
len_B_mem_b_b = length(member_B_b_b);
bb1=[bb1_a(1:len_A_mem_b_b) bb1_b(1:len_B_mem_b_b)];
bb2=[bb2_a(1:len_A_mem_b_b) bb2_b(1:len_B_mem_b_b)];
bb3=[bb3_a(1:len_A_mem_b_b) bb3_b(1:len_B_mem_b_b)];
bb4=[bb4_a(1:len_A_mem_b_b) bb4_b(1:len_B_mem_b_b)];
b_tmp=[bb1;bb2;bb3;bb4];
b_tmp_=sortrows(b_tmp',1);
b_tmp=b_tmp_';
bb1=b_tmp(1,:);
bb2=b_tmp(2,:);
bb3=b_tmp(3,:);
bb4=b_tmp(4,:);
%�������ʱ������ϵͳ���ܹ˿���
%*****************************************
%������
%*****************************************
%�����ڷ���ʱ���ڣ�����ϵͳ�����й˿͵ĵ���ʱ�̺���
%��ʱ������ͼ��stairs�����ƶ�ά����ͼ��
% figure;
% stairs(0:arr_num_b,[0 bb1],'o-','linewidth',1, 'MarkerFaceColor','g','markersize',2);
% hold on;
% stairs(0:arr_num_b,[0 bb3 ],'o-','linewidth',1, 'MarkerFaceColor','y','markersize',2);
% legend('B_b arriving time','B_b leaving time');
%     set(gca,'Box','off','TickDir','out','Ticklength',[.02 .02],...
%     'XMinorTick','on','YMinorTick','on','YGrid','on',...
%     'XColor',[.3 .3 .3],'YColor',[.3 .3 .3],'LineWidth',1);
% hold off;
% grid on;
%�����ڷ���ʱ���ڣ�����ϵͳ�����й˿͵�ͣ��ʱ��͵�
%��ʱ������ͼ��plot�����ƶ�ά����ͼ��
B_cost_time_b=zeros(1,arr_num); %��¼ÿ��������ϵͳ����ʱ��
B_wait_time_b=zeros(1,arr_num); %��¼ÿ��������ϵͳ�ȴ�ʱ��
for i=1:arr_num
  B_cost_time_b(i)=bb3(i)-bb1(i); %��i��������ϵͳ����ʱ��
  B_wait_time_b(i)=bb2(i)-bb1(i); %��i��������ϵͳ�ȴ�ʱ��
end

%%
%demo
% plot(x,y,'o-','linewidth',2, 'MarkerFaceColor','g','markersize',4);
% title('�����ٶ�')
%     set(gca,'Box','off','TickDir','out','Ticklength',[.02 .02],...
%     'XMinorTick','on','YMinorTick','on','YGrid','on',...
%     'XColor',[.3 .3 .3],'YColor',[.3 .3 .3],'LineWidth',1);
%% x_���
% T1=bb3_a(len_A_mem_b_b)+bb3_b(len_B_mem_b_b)%��ʱ��
% p1=sum(state_a_b(member_A_b_b))/T1+sum(state_a_b(member_A_b_b))/T1 %������
% avert1=sum(B_wait_time_b)/(sum(state_a_b(member_A_b_b)))%ÿ������ϵͳƽ������ʱ��
% fprintf('B passenager averager cost time%fs\n',avert1);
% fprintf('B system working intensity %f\n',p1);

%% D ������
if sum(ba5_a)+sum(ba5_b)==0
    ba5_a(1)=1;
end

da1_b=zeros(1,sum(bb5_a)+sum(bb5_b));
index=find(bb5_a==1);
for i=1:sum(bb5_a)
    da1_b(i)=bb3_a(index(i));
end
index=find(bb5_b==1);n=1;
for i=sum(bb5_a)+1:sum(bb5_a)+sum(bb5_b)
    da1_b(i)=bb3_b(index(n));
    n=n+1;
end
da1_b=sort(da1_b);

