#include "alldata.h"
#include "calexpression.cpp"

#define DBG qDebug()<<__FILE__<<" "<<__FUNCTION__<<"()"<<__LINE__

AllData::AllData()
{

    length_VS1 = 0;
    length_VS2 = 0;
	length_Jingdu = 0;

    vs1_ok = false;
    vs2_ok = false;
    jingdu_step = 0;

    initValue_VS1_modeVS = 1000;
    initValue_VS2_modeVS = 1000;

    initValue_VS1_modeJingdu = 1000;
    initValue_VS2_modeJingdu = 1000;
    initValue_Yinliu_modeJingdu = 1000;

    curWorker = "hoorayitt.";
	JingduCount = 0;
    curAction = Action_die;
}

bool AllData::returnData_FromVS(Mode mode, QVector<double> **a,
                                QVector<QString> **b){

    if(mode==Mode_VS1){
        a[0] = &original_VS1;
        a[1] = &b_VS1;
        a[2] = &r_VS1;
        a[3] = &differential_VS1;
        a[4] = &a_VS1;
        a[5] = &adjust_VS1;
        a[6] = &final_VS1;
        b[0] = &status_VS1;
        b[1] = &date_VS1;
        b[2] = &worker_VS1;
    }
    else if(mode==Mode_VS2){
        a[0] = &original_VS2;
        a[1] = &b_VS2;
        a[2] = &r_VS2;
        a[3] = &differential_VS2;
        a[4] = &a_VS2;
        a[5] = &adjust_VS2;
        a[6] = &final_VS2;
        b[0] = &status_VS2;
        b[1] = &date_VS2;
        b[2] = &worker_VS2;
    }
    else if(mode==Mode_Jingdu){
        a[0] = &originalVS1_Jingdu;
        a[1] = &originalVS1_Jingdu;
        a[2] = &originalFlow_Jingdu;
                                      
        a[3] = &adjustVS1_Jingdu;
        a[4] = &adjustVS2_Jingdu;
        a[5] = &adjustFlow_Jingdu;
                                      
        a[6] = &b0_Jingdu;
        a[7] = &r0_Jingdu;
        a[8] = &differential0_Jingdu;
        a[9] = &a0_Jingdu;
        a[10] =&accuracy0_Jingdu;
                                      
        a[11] =&b1_Jingdu;
        a[12] =&r1_Jingdu;
        a[13] =&differential1_Jingdu;
        a[14] =&a1_Jingdu;
        a[15] =&accuracy1_Jingdu;
                                      
        b[0] = &status_Jingdu;
        b[1] = &date_Jingdu;
        b[2] = &worker_Jingdu;
    }
    else{
        return false;
    }

    return true;
}

double AllData::cal_adjustValue(){
    QString expression;
    if(curMode==Mode_VS1)
    {
        expression = Expression_VS1;
        DBG<<length_VS1<<" "<<expression;
        double a = a_VS1.back();
        double b = b_VS1.back();
        double r = r_VS1.back();


        expression = expression.split("a").join(QString::number(a));
        expression = expression.split("b").join(QString::number(b));
        expression = expression.split("r").join(QString::number(r));

        return cal_expression(expression);

    }
    else if(curMode==Mode_VS2)
    {
        expression = Expression_VS2;

        double a = a_VS2.back();
        double b = b_VS2.back();
        double r = r_VS2.back();


        expression = expression.split("a").join(QString::number(a));
        expression = expression.split("b").join(QString::number(b));
        expression = expression.split("r").join(QString::number(r));

        return cal_expression(expression);

    }
    else if(curMode==Mode_Jingdu)
    {
        //expression = Expression_VS2;
        //return cal_expression(expression);
		return 1;
    }

}

double AllData::cal_expression(QString e){//use stack to calculate the expression
    e.append('#');
    QByteArray ba = e.toLatin1();
    double r = calExpression(ba.data());
    DBG<<"expression is"<<ba.data();
    DBG<<"expression final is :"<<r;

    return r;
}

void AllData::push_a(double a){
    if(curMode==Mode_VS1)
    {
        a_VS1.push_back(a);
        if(length_VS1 < a_VS1.size())
            length_VS1 = a_VS1.size();
    }
    else if(curMode==Mode_VS2)
    {
        a_VS2.push_back(a);
        if(length_VS2 < a_VS2.size())
            length_VS2 = a_VS2.size();
    }
    else if(curMode==Mode_Jingdu)
    {
		if(a1_Jingdu.size()<a0_Jingdu.size())
			a1_Jingdu.push_back(a);
		else
			a0_Jingdu.push_back(a);

        if(length_Jingdu < a0_Jingdu.size())
            length_Jingdu = a0_Jingdu.size();
    }
}

void AllData::push_b(double b){
    if(curMode==Mode_VS1)
    {
        b_VS1.push_back(b);
        if(length_VS1 < b_VS1.size())
            length_VS1 = b_VS1.size();
        if(VSCount!=2)
            original_VS1.push_back(initValue_VS1_modeVS);
        else
            original_VS1.push_back(final_VS1.back());
        worker_VS1.push_back(curWorker);
        date_VS1.push_back(QDate::currentDate().toString("yy-MM-dd"));
    }
    else if(curMode==Mode_VS2)
    {
        b_VS2.push_back(b);
        if(length_VS2 < b_VS2.size())
            length_VS2 = b_VS2.size();
        if(VSCount!=2)
            original_VS2.push_back(initValue_VS2_modeVS);
        else
            original_VS2.push_back(final_VS2.back());
        worker_VS2.push_back(curWorker);
        date_VS2.push_back(QDate::currentDate().toString("yy-MM-dd"));
    }
    else if(curMode==Mode_Jingdu)
    {
		if(b1_Jingdu.size()<b0_Jingdu.size())
			b1_Jingdu.push_back(b);
		else{
			originalVS1_Jingdu.push_back(initValue_VS1_modeJingdu);
			originalVS2_Jingdu.push_back(initValue_VS2_modeJingdu);
			originalFlow_Jingdu.push_back(initValue_Yinliu_modeJingdu);
			b0_Jingdu.push_back(b);
			worker_Jingdu.push_back(curWorker);
			date_Jingdu.push_back(QDate::currentDate().toString("yy-MM-dd"));
		}
        if(length_Jingdu < b0_Jingdu.size())
            length_Jingdu = b0_Jingdu.size();


    }

}


void AllData::push_r(double r){
    if(curMode==Mode_VS1)
    {
        r_VS1.push_back(r);
        if(length_VS1 < r_VS1.size())
            length_VS1 = r_VS1.size();
        differential_VS1.push_back(r-b_VS1[length_VS1-1]);
        double result = cal_adjustValue();
        adjust_VS1.push_back(result);
        if(VSCount==0)
        {
            final_VS1.push_back(result);
            status_VS1.push_back("1");
        }
        else if(VSCount==1){
            double pre = final_VS1.at(final_VS1.size()-1);
            final_VS1.push_back((result+pre)/2);
			averageValue = final_VS1.back();//return to A
            status_VS1.push_back("2");
        }
        else if(VSCount==2)
        {
            final_VS1.push_back(result);
            int a = final_VS1.back();
            int b = original_VS1.back() - a;
            if(b<0)
                b = -b;
            if(b<10000)
            {
                status_VS1.push_back(QString("OK"));
                vs1_ok = true;
            }
            else
                status_VS1.push_back(QString("%1 <> %2").arg(final_VS1.back()).arg(original_VS1.back()));

            VSCount = 0;
            return ;
        }
        VSCount ++;

    }
    else if(curMode==Mode_VS2)
    {
        r_VS2.push_back(r);
        if(length_VS2 < r_VS2.size())
            length_VS2 = r_VS2.size();
        differential_VS2.push_back(r-b_VS2[length_VS2-1]);
        double result = cal_adjustValue();
        adjust_VS2.push_back(result);
        if(VSCount==0)
        {
            final_VS2.push_back(result);
            status_VS2.push_back("1");
        }
        else if(VSCount==1){
            double pre = final_VS2.at(final_VS2.size()-1);
            final_VS2.push_back((result+pre)/2);
			averageValue = final_VS2.back();//return to A
            status_VS2.push_back("2");
        }
        else if(VSCount==2)
        {
            final_VS2.push_back(result);
            int a = final_VS2.back();
            int b = original_VS2.back() - a;
            if(b<0)
                b = -b;
            if(b<5000)
            {
                status_VS2.push_back(QString("OK"));
                vs2_ok = true;
            }
            else
                status_VS2.push_back(QString("%1 <> %2").arg(final_VS2.back()).arg(original_VS2.back()));

            VSCount = 0;
            return ;
        }
        VSCount ++;
    }
    else if(curMode==Mode_Jingdu)
    {
		if(r1_Jingdu.size()<r0_Jingdu.size()){
			r1_Jingdu.push_back(r);
			differential1_Jingdu.push_back(r-b1_Jingdu.back());
			accuracy1_Jingdu.push_back(100*(a1_Jingdu.back()-differential1_Jingdu.back())/differential1_Jingdu.back());
			cal_finalValues_JingduMode();

		}
		else{
			r0_Jingdu.push_back(r);
			differential0_Jingdu.push_back(r-b0_Jingdu.back());
			accuracy0_Jingdu.push_back(100*(a0_Jingdu.back()-differential0_Jingdu.back())/differential0_Jingdu.back());

		}
        if(length_Jingdu < r0_Jingdu.size())//r0_len >= r1
            length_Jingdu = r0_Jingdu.size();

    }

}

void AllData::complete(){//let the length equal
    //status is "STOP"
    if(curMode==Mode_VS1){
        a_VS1.size()<length_VS1 &&  (a_VS1.push_back(0),1); //短路写法 ； 后面表达式值为void不行,所以要加个1
        b_VS1.size()<length_VS1 &&  (b_VS1.push_back(0),1);
        r_VS1.size()<length_VS1 &&  (r_VS1.push_back(0),1);
        original_VS1.size()<length_VS1 &&  (original_VS1.push_back(0),1);
        differential_VS1.size()<length_VS1 &&  (differential_VS1.push_back(0),1);
        adjust_VS1.size()<length_VS1 &&  (adjust_VS1.push_back(0),1);
        final_VS1.size()<length_VS1 &&  (final_VS1.push_back(0),1);
        status_VS1.size()<length_VS1 &&  (status_VS1.push_back("STOP"),1);
    }
    else if(curMode==Mode_VS2){
        a_VS2.size()<length_VS2 &&  (a_VS2.push_back(0),1);
        b_VS2.size()<length_VS2 &&  (b_VS2.push_back(0),1);
        r_VS2.size()<length_VS2 &&  (r_VS2.push_back(0),1);
        original_VS2.size()<length_VS2 &&  (original_VS2.push_back(0),1);
        differential_VS2.size()<length_VS2 &&  (differential_VS2.push_back(0),1);
        adjust_VS2.size()<length_VS2 &&  (adjust_VS2.push_back(0),1);
        final_VS2.size()<length_VS2 &&  (final_VS2.push_back(0),1);
        status_VS2.size()<length_VS2 &&  (status_VS2.push_back("STOP"),1);
    }
    else if(curMode==Mode_Jingdu){

		originalVS1_Jingdu.size()<length_Jingdu && (originalVS1_Jingdu.push_back(0),1);
        originalVS2_Jingdu.size()<length_Jingdu &&  ( originalVS2_Jingdu.push_back(0),1);
        originalFlow_Jingdu.size()<length_Jingdu &&  (originalFlow_Jingdu.push_back(0),1);

        adjustVS1_Jingdu.size()<length_Jingdu &&  (adjustVS1_Jingdu.push_back(0),1);
        adjustVS2_Jingdu.size()<length_Jingdu &&  (adjustVS2_Jingdu.push_back(0),1);
        adjustFlow_Jingdu.size()<length_Jingdu &&  (adjustFlow_Jingdu.push_back(0),1);

        b0_Jingdu.size()<length_Jingdu &&  (b0_Jingdu.push_back(0),1);
        r0_Jingdu.size()<length_Jingdu &&  (r0_Jingdu.push_back(0),1);
        differential0_Jingdu.size()<length_Jingdu &&  (differential0_Jingdu.push_back(0),1);
        a0_Jingdu.size()<length_Jingdu &&  (a0_Jingdu.push_back(0),1);
        accuracy0_Jingdu.size()<length_Jingdu &&  (accuracy0_Jingdu.push_back(0),1);

        b1_Jingdu.size()<length_Jingdu &&  (b1_Jingdu.push_back(0),1);
        r1_Jingdu.size()<length_Jingdu &&  (r1_Jingdu.push_back(0),1);
        differential1_Jingdu.size()<length_Jingdu &&  (differential1_Jingdu.push_back(0),1);
        a1_Jingdu.size()<length_Jingdu &&  (a1_Jingdu.push_back(0),1);
        accuracy1_Jingdu.size()<length_Jingdu &&  (accuracy1_Jingdu.push_back(0),1);

		status_Jingdu.size()<length_Jingdu &&  (status_Jingdu.push_back("STOP"),1);

	}	
		

}

void AllData::cal_finalValues_JingduMode(){

	//A4=A1*(1.008-A16/100)
	//A5=A2*(1.008-A16/100)
	//A6=A3-(A11+(0.8-A16)-1.2)/100
	double A11 = accuracy0_Jingdu.back();
	double A16 = accuracy1_Jingdu.back();
	double A4 = originalVS1_Jingdu.back() * (1.008 - A16/100);
	double A5 = originalVS2_Jingdu.back() * (1.008 - A16/100);
	double A6 = originalFlow_Jingdu.back() - (A11 + (0.8 - A16) - 1.2)/100;

	adjustVS1_Jingdu.push_back(A4);
	adjustVS2_Jingdu.push_back(A5);
	adjustFlow_Jingdu.push_back(A6);

	//第一次判断
	if(jingdu_step == 0){ 

		if(JingduCount==2){
			DBG<<"判断一次";
			bool flag = false;
			status_Jingdu.push_back("前两组不满足");
			
		}
		else if(JingduCount==3){
			DBG<<"再判断一次";


			//TODO::假设成功,修改initValue_VS1_modeVS等等
			status_Jingdu.push_back("step1成功");
			jingdu_step ++;
			JingduCount = -2;
		}
		else{
			status_Jingdu.push_back(QString("step%1-%2").arg(jingdu_step+1).arg(JingduCount+1));
		}

	}
	//第二次判断
	else{
		if(JingduCount==1){
			DBG<<"判断一次";
			bool flag = false;
			
			status_Jingdu.push_back("前两组不满足");
		}
		else if(JingduCount==2){
			DBG<<"再判断一次";


			//TODO::假设成功,修改initValue_VS1_modeVS等等
			jingdu_step ++;
			status_Jingdu.push_back("成功");
			return;
		}
		else{
			status_Jingdu.push_back(QString("step%1-%2").arg(jingdu_step+1).arg(JingduCount+1));
		}
	}
	JingduCount ++ ;
		/*
        if(VSCount==0)
        {
            final_VS2.push_back(result);
            status_VS2.push_back("1");
        }
        else if(VSCount==1){
            double pre = final_VS2.at(final_VS2.size()-1);
            final_VS2.push_back((result+pre)/2);
			averageValue = final_VS2.back();//return to A
            status_VS2.push_back("2");
        }
        else if(VSCount==2)
        {
            final_VS2.push_back(result);
            int a = final_VS2.back();
            int b = original_VS2.back() - a;
            if(b<0)
                b = -b;
            if(b<5000)
            {
                status_VS2.push_back(QString("OK"));
                vs2_ok = true;
            }
            else
                status_VS2.push_back(QString("%1 <> %2").arg(final_VS2.back()).arg(original_VS2.back()));

            VSCount = 0;
            return ;
        }
        VSCount ++;
		*/
}
