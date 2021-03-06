#include "alldata.h"
#include "calexpression.cpp"

#define DBG qDebug()<<__FILE__<<" "<<__FUNCTION__<<"()"<<__LINE__

AllData::AllData()
{
    updateFlag = false;

    length_VS1 = 0;
    length_VS2 = 0;
    length_Jingdu = 0;

    vs1_ok = false;
    vs2_ok = false;
    jingdu_step = 0;

    initValue_VS1_modeVS = 0;
    initValue_VS2_modeVS = 0;

	minWater = 0;
	range_vsmode = 0;

    initValue_VS1_modeJingdu = 0;
    initValue_VS2_modeJingdu = 0;
    initValue_Yinliu_modeJingdu = 0;

	Threshold1 = 0;
	Threshold2 = 0;

	range1 = 0;
	range2 = 0;
	range3 = 0;
	range4 = 0;

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
        a[1] = &originalVS2_Jingdu;
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
        DBG<<length_VS1<<" ======================="<<expression;
        double a = a_VS1.back();
        double b = b_VS1.back();
        double r = r_VS1.back();
        double c = original_VS1.back();


        expression = expression.split("a").join(QString::number(a));
        expression = expression.split("b").join(QString::number(b));
        expression = expression.split("r").join(QString::number(r));
        expression = expression.split("c").join(QString::number(c));

        return cal_expression(expression);

    }
    else if(curMode==Mode_VS2)
    {
        expression = Expression_VS2;

        double a = a_VS2.back();
        double b = b_VS2.back();
        double r = r_VS2.back();
        double c = original_VS2.back();


        expression = expression.split("a").join(QString::number(a));
        expression = expression.split("b").join(QString::number(b));
        expression = expression.split("r").join(QString::number(r));
        expression = expression.split("c").join(QString::number(c));

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
	Result *s= calExpression(ba.data());
	DBG<<s->flag<<s->msg<<s->value;
    double r = s->value;
    if(s->flag){
		QMessageBox::warning(NULL,"公式计算错误",QString(s->msg));
		complete();
		curAction = Action_die;
		return 0;
	}

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
        original_VS1.push_back(initValue_VS1_modeVS);
        worker_VS1.push_back(curWorker);
        date_VS1.push_back(QDate::currentDate().toString("yy-MM-dd"));
    }
    else if(curMode==Mode_VS2)
    {
        b_VS2.push_back(b);
        if(length_VS2 < b_VS2.size())
            length_VS2 = b_VS2.size();
        original_VS2.push_back(initValue_VS2_modeVS);
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
        differential_VS1.push_back(myAbs(r-b_VS1.back()));
        double result = cal_adjustValue();
        double pre = adjust_VS1.back();
        adjust_VS1.push_back(result);
        if(curAction==Action_die) return;
        if(VSCount==0)
        {
            final_VS1.push_back(0);
            status_VS1.push_back("1");
        }
        else if(VSCount==1){

            if(myAbs(pre-result)<=range_vsmode){
                final_VS1.push_back((result+pre)/2);
                averageValue = final_VS1.back();//return to A
                status_VS1.push_back("OK");
                vs1_ok = true;
                VSCount = 0;
                return ;
            }
            else{
                final_VS1.push_back(0);
                status_VS1.push_back("2");
            }
        }
        else if(VSCount==2)
        {

            if(myAbs(pre-result)<=range_vsmode){
                final_VS1.push_back((result+pre)/2);
                averageValue = final_VS1.back();//return to A
                status_VS1.push_back("OK");
                vs1_ok = true;
                VSCount = 0;
                return ;
            }
            else{
                final_VS1.push_back(0);
                status_VS1.push_back(QString("|%1-%2|=%3>%4").arg(pre).arg(result).arg(myAbs(pre-result)).arg(range_vsmode));
                vs1_ok = false;
            }
        }
        VSCount ++;

    }
    else if(curMode==Mode_VS2)
    {
        r_VS2.push_back(r);
        if(length_VS2 < r_VS2.size())
            length_VS2 = r_VS2.size();
        differential_VS2.push_back(myAbs(r-b_VS2.back()));
        double result = cal_adjustValue();
        double pre = adjust_VS2.back();
        adjust_VS2.push_back(result);
        if(curAction==Action_die) return;
        if(VSCount==0)
        {
            final_VS2.push_back(0);
            status_VS2.push_back("1");
        }
        else if(VSCount==1){

            if(myAbs(pre-result)<=range_vsmode){
                final_VS2.push_back((result+pre)/2);
                averageValue = final_VS2.back();//return to A
                status_VS2.push_back("OK");
                vs2_ok = true;
                VSCount = 0;
                return ;
            }
            else{
                final_VS2.push_back(0);
                status_VS2.push_back("2");
            }
        }
        else if(VSCount==2)
        {

            if(myAbs(pre-result)<=range_vsmode){
                final_VS2.push_back((result+pre)/2);
                averageValue = final_VS2.back();//return to A
                status_VS2.push_back("OK");
                vs2_ok = true;
                VSCount = 0;
                return ;
            }
            else{
                final_VS2.push_back(0);
                status_VS2.push_back(QString("|%1-%2|=%3>%4").arg(pre).arg(result).arg(myAbs(pre-result)).arg(range_vsmode));
                vs2_ok = false;
            }
        }
        VSCount ++;

    }
    else if(curMode==Mode_Jingdu)
    {
		if(r1_Jingdu.size()<r0_Jingdu.size()){ //注入值  后半部分
			r1_Jingdu.push_back(r);
			differential1_Jingdu.push_back(myAbs(r-b1_Jingdu.back()));
			accuracy1_Jingdu.push_back(100*(a1_Jingdu.back()-differential1_Jingdu.back())/differential1_Jingdu.back());
			cal_finalValues_JingduMode(); //增加备注

		}
		else{ //引流值 前半部分
			r0_Jingdu.push_back(r);
			differential0_Jingdu.push_back(myAbs(r-b0_Jingdu.back()));
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
	//A6=A3-(A11+(0.8-A16)-1.5)/100
	double A11 = accuracy0_Jingdu.back();
	double A16 = accuracy1_Jingdu.back();
	double A4 = originalVS1_Jingdu.back() * (1.008 - A16/100);
	double A5 = originalVS2_Jingdu.back() * (1.008 - A16/100);
	double A6 = originalFlow_Jingdu.back() - (A11 + (0.8 - A16) - 1.5)/100;

	adjustVS1_Jingdu.push_back(A4);
	adjustVS2_Jingdu.push_back(A5);
	adjustFlow_Jingdu.push_back(A6);


	//第0轮不做判断
	if(!jingdu_step){
		status_Jingdu.push_back(QString("热机"));
		jingdu_step++;
		return;
	}
	if(JingduCount<1){
		status_Jingdu.push_back(QString("step%1-%2").arg(jingdu_step).arg(JingduCount+1));
		JingduCount++;
		return;
	}

	/*
	 * 参数：	1：最新的两组
	 *			2：距离较远的两组
	 */
	if(JingduCount==1){//第二次
		if(StableJudge(1)){
			if(jingdu_step==1){//第一轮稳定
                if(RangeJudge(1)){//满足range
                    updateValue(1);
                    status_Jingdu.push_back(QString("step%1-%2: 精度调试OK（本次和上次满足）").arg(jingdu_step).arg(JingduCount+1));
                    jingdu_step = -2;
                }
                else{
                    updateValue(1);
                    status_Jingdu.push_back(QString("step%1-%2: 这两组稳定,但不满足range条件，").arg(jingdu_step).arg(JingduCount+1));
                    jingdu_step++;
                    JingduCount=0;
                }
            }
			else{//第 >1 轮稳定
				if(RangeJudge(1)){//满足range
					updateValue(1);
					status_Jingdu.push_back(QString("step%1-%2: 精度调试OK（本次和上次满足）").arg(jingdu_step).arg(JingduCount+1));
					jingdu_step = -2;
				}
				else{//不满足range，再测一组
					updateValue(1);
					status_Jingdu.push_back(QString("step%1-%2: 这两组稳定，但不满足range条件，开始新的一轮").arg(jingdu_step).arg(JingduCount+1));
					jingdu_step++;
					JingduCount=0;
				}
			}
		}
		else{//再测一轮
			status_Jingdu.push_back(QString("step%1-%2: 这两组不稳定,再测一次").arg(jingdu_step).arg(JingduCount+1));
			JingduCount++;
		}
	}
	else if(JingduCount==2){//第三次
		if(jingdu_step==1){//第一轮
			if(StableJudge(1)){//距离为1的两组ok
                if(RangeJudge(1)){//满足range
                    updateValue(1);
                    status_Jingdu.push_back(QString("step%1-%2: 精度调试OK（本次和上次满足）").arg(jingdu_step).arg(JingduCount+1));
                    jingdu_step = -2;
                    return;
                }
                else{
                    updateValue(1);
                    status_Jingdu.push_back(QString("step%1-%2: 上组和本组稳定,但不满足range条件，").arg(jingdu_step).arg(JingduCount+1));
                    jingdu_step++;
                    JingduCount=0;
                }
			}
			else if(StableJudge(2)){//距离为2的两组ok
                if(RangeJudge(2)){//满足range
                    updateValue(2);
                    status_Jingdu.push_back(QString("step%1-%2: 精度调试OK（本次和上上次满足）").arg(jingdu_step).arg(JingduCount+1));
                    jingdu_step = -2;
                return;
                }
                else{
                    updateValue(2);
                    status_Jingdu.push_back(QString("step%1-%2: 上上组和本组稳定,但不满足range条件，").arg(jingdu_step).arg(JingduCount+1));
                    jingdu_step++;
                    JingduCount=0;
                }
			}
			else{//这三组都不行
				status_Jingdu.push_back(QString("step%1-%2: 精度不稳，请检查").arg(jingdu_step).arg(JingduCount+1));
				JingduCount=0;
				jingdu_step = -1;
			}
		}
		else{//不是第一轮，>1
			if(!StableJudge(1) && !StableJudge(2)){
				status_Jingdu.push_back(QString("step%1-%2: 精度不稳，请检查").arg(jingdu_step).arg(JingduCount+1));
				JingduCount=0;
				jingdu_step = -1;
				return;
			}

			if(StableJudge(1) && RangeJudge(1)){//距离为1的两组ok
				updateValue(1);
				status_Jingdu.push_back(QString("step%1-%2: 精度调试OK（本次和上次满足）").arg(jingdu_step).arg(JingduCount+1));
				jingdu_step = -2;
				return;
			}

			if(StableJudge(2) && RangeJudge(2)){//距离为2的两组ok
				updateValue(2);
				status_Jingdu.push_back(QString("step%1-%2: 精度调试OK（本次和上上次满足）").arg(jingdu_step).arg(JingduCount+1));
				jingdu_step = -2;
				return;
			}

			if(StableJudge(1)){
				updateValue(1);
			}
			else if(StableJudge(2)){
				updateValue(2);
			}

			status_Jingdu.push_back(QString("step%1-%2: 这三组稳定，但不满足range条件，开始新的一轮").arg(jingdu_step).arg(JingduCount+1));
			JingduCount=0;
			jingdu_step++;
		}
	}
	else{
		status_Jingdu.push_back(QString("不会出现这一句"));
		jingdu_step = -1;
	}


}

double AllData::myAbs(double b){
	if(b<0) return -b;
	return b;
}


void AllData::updateValue(int distance){
			//3.按照条件1，B、C、D两两判断，选择满足条件1的两组数据(比如B、D两组)，则E1=(B4+D4)/2,E2=(B5+D5)/2,E3=(B6+D6)/2。
    updateFlag = true;
	if(distance==1){
		initValue_VS1_modeJingdu = (adjustVS1_Jingdu[length_Jingdu-2] + adjustVS1_Jingdu[length_Jingdu-1])/2;
		initValue_VS2_modeJingdu = (adjustVS2_Jingdu[length_Jingdu-2] + adjustVS2_Jingdu[length_Jingdu-1])/2;
		initValue_Yinliu_modeJingdu = (adjustFlow_Jingdu[length_Jingdu-2] + adjustFlow_Jingdu[length_Jingdu-1])/2;
	}
	else{
		initValue_VS1_modeJingdu = (adjustVS1_Jingdu[length_Jingdu-3] + adjustVS1_Jingdu[length_Jingdu-1])/2;
		initValue_VS2_modeJingdu = (adjustVS2_Jingdu[length_Jingdu-3] + adjustVS2_Jingdu[length_Jingdu-1])/2;
		initValue_Yinliu_modeJingdu = (adjustFlow_Jingdu[length_Jingdu-3] + adjustFlow_Jingdu[length_Jingdu-1])/2;
	}
}

bool AllData::StableJudge(int distance){
		/*
			1.判断|B11-C11|<Threshold1？且|B16-C16|<Threshold2？(Threshold的值可编辑)，则D1=(B4+C4)/2,D2=(B5+C5)/2,D3=(B6+C6)/2。
			2.不满足条件1，则重复步骤二 D1~3与A1~3相同，再测一组(D组)。
			3.按照条件1，B、C、D两两判断，选择满足条件1的两组数据(比如B、D两组)，则E1=(B4+D4)/2,E2=(B5+D5)/2,E3=(B6+D6)/2。
			4.若不满足条件3，则暂停该设备组并在界面上提示“精度不稳，请检查”。"
		*/
	double B11 = accuracy0_Jingdu[length_Jingdu-3];
	double B16 = accuracy1_Jingdu[length_Jingdu-3];
	double C11 = accuracy0_Jingdu[length_Jingdu-2];
	double C16 = accuracy1_Jingdu[length_Jingdu-2];
	double D11 = accuracy0_Jingdu.back();
	double D16 = accuracy1_Jingdu.back();

	if(distance==1){
        qDebug()<<"=============>  StableJudge"<<C11<<" "<<D11<<" "<<C16<<" "<<D16;
		if(myAbs(D11-C11)<Threshold1 && myAbs(D16-C16)<Threshold2) return true;
	}
	else{
        qDebug()<<"=============>  StableJudge"<<B11<<" "<<D11<<" "<<B16<<" "<<D16;
		if(myAbs(D11-B11)<Threshold1 && myAbs(D16-B16)<Threshold2) return true;
	}
	return false;
}

bool AllData::RangeJudge(int distance){
	/*  1.按步骤四的方式得出满足条件的两组数据（比如E、F组）。
		2.判断E11和F11∈(Range1, Range2)？ E16和F16∈(Range3, Range4)？(Range的值可编辑)则流程结束。
		3.若不满足条件2，则重复步骤五、六、七。"  */
	double B11 = accuracy0_Jingdu[length_Jingdu-3];
	double B16 = accuracy1_Jingdu[length_Jingdu-3];
	double C11 = accuracy0_Jingdu[length_Jingdu-2];
	double C16 = accuracy1_Jingdu[length_Jingdu-2];
	double D11 = accuracy0_Jingdu.back();
	double D16 = accuracy1_Jingdu.back();

	if(distance==1){
        DBG<<"=============>  RangeJudge"<<C11<<" "<<D11<<" "<<C16<<" "<<D16;
		if(min(D11,C11)>=range1 && max(D11,C11)<=range2 && min(D16,C16)>=range3 && max(D16,C16)<=range4) return true;
	}
	else{
        DBG<<"=============>  RangeJudge"<<B11<<" "<<D11<<" "<<B16<<" "<<D16;
		if(min(B11,D11)>=range1 && max(B11,D11)<=range2 && min(B16,D16)>=range3 && max(B16,D16)<=range4) return true;
	}
	return false;
}


void AllData::saveData(FILE *fp){
    ///////////////////////////////////////////////////////////VS1
    saveDoubleVector(&original_VS1, fp);
    saveDoubleVector(&b_VS1, fp);
    saveDoubleVector(&r_VS1, fp);
    saveDoubleVector(&differential_VS1, fp);
    saveDoubleVector(&a_VS1, fp);
    saveDoubleVector(&adjust_VS1, fp);
    saveDoubleVector(&final_VS1, fp);

    saveStringVector(&status_VS1, fp);
    saveStringVector(&date_VS1, fp);
    saveStringVector(&worker_VS1, fp);

    fwrite(&length_VS1, sizeof(int),1,fp);


    ////////////////////////////////////////////////////////////VS2
    saveDoubleVector(&original_VS2, fp);
    saveDoubleVector(&b_VS2, fp);
    saveDoubleVector(&r_VS2, fp);
    saveDoubleVector(&differential_VS2, fp);
    saveDoubleVector(&a_VS2, fp);
    saveDoubleVector(&adjust_VS2, fp);
    saveDoubleVector(&final_VS2, fp);

    saveStringVector(&status_VS2, fp);
    saveStringVector(&date_VS2, fp);
    saveStringVector(&worker_VS2, fp);

    fwrite(&length_VS2, sizeof(int),1,fp);


    ////////////////////////////////////////////////////////////JD

    saveDoubleVector(&originalVS1_Jingdu, fp);
    saveDoubleVector(&originalVS2_Jingdu, fp);
    saveDoubleVector(&originalFlow_Jingdu, fp);
    saveDoubleVector(&adjustVS1_Jingdu, fp);
    saveDoubleVector(&adjustVS2_Jingdu, fp);
    saveDoubleVector(&adjustFlow_Jingdu, fp);
    saveDoubleVector(&b0_Jingdu, fp);
    saveDoubleVector(&r0_Jingdu, fp);
    saveDoubleVector(&differential0_Jingdu, fp);
    saveDoubleVector(&a0_Jingdu, fp);
    saveDoubleVector(&accuracy0_Jingdu, fp);
    saveDoubleVector(&b1_Jingdu, fp);
    saveDoubleVector(&r1_Jingdu, fp);
    saveDoubleVector(&differential1_Jingdu, fp);
    saveDoubleVector(&a1_Jingdu, fp);
    saveDoubleVector(&accuracy1_Jingdu, fp);

    //备注
    saveStringVector(&status_Jingdu, fp);
    saveStringVector(&date_Jingdu, fp);
    saveStringVector(&worker_Jingdu, fp);

    fwrite(&length_Jingdu, sizeof(int),1,fp);

    //===========================================================
    //  精度调试
    //===========================================================
    fwrite(&Threshold1, sizeof(double), 1, fp);
    fwrite(&Threshold2, sizeof(double), 1, fp);
    fwrite(&range1, sizeof(double), 1, fp);
    fwrite(&range2, sizeof(double), 1, fp);
    fwrite(&range3, sizeof(double), 1, fp);
    fwrite(&range4, sizeof(double), 1, fp);
    fwrite(&initValue_VS1_modeJingdu, sizeof(double), 1, fp);
    fwrite(&initValue_VS2_modeJingdu, sizeof(double), 1, fp);
    fwrite(&initValue_Yinliu_modeJingdu, sizeof(double), 1, fp);
}

void AllData::saveDoubleVector(QVector<double> *vec, FILE *fp){
    int len = vec->size();
    fwrite(&len, sizeof(int), 1, fp);
    fwrite(vec->data(), sizeof(double),len,fp);
}

void AllData::saveStringVector(QVector<QString> *vec, FILE *fp){
    int len = vec->size(), l;
    fwrite(&len, sizeof(int), 1, fp);
    for(int i=0;i<len;i++){
        l = vec->at(i).length();
        fwrite(&l, sizeof(int),1,fp);
        fwrite(vec->at(i).data(), sizeof(QChar),l,fp);
    }
}

void AllData::readData(FILE *fp){

    readDoubleVector(&original_VS1, fp);
    readDoubleVector(&b_VS1, fp);
    readDoubleVector(&r_VS1, fp);
    readDoubleVector(&differential_VS1, fp);
    readDoubleVector(&a_VS1, fp);
    readDoubleVector(&adjust_VS1, fp);
    readDoubleVector(&final_VS1, fp);

    readStringVector(&status_VS1, fp);
    readStringVector(&date_VS1, fp);
    readStringVector(&worker_VS1, fp);

    fread(&length_VS1, sizeof(int),1,fp);

    ////////////////////////////////////////////////////////////VS2
    readDoubleVector(&original_VS2, fp);
    readDoubleVector(&b_VS2, fp);
    readDoubleVector(&r_VS2, fp);
    readDoubleVector(&differential_VS2, fp);
    readDoubleVector(&a_VS2, fp);
    readDoubleVector(&adjust_VS2, fp);
    readDoubleVector(&final_VS2, fp);

    readStringVector(&status_VS2, fp);
    readStringVector(&date_VS2, fp);
    readStringVector(&worker_VS2, fp);

    fread(&length_VS2, sizeof(int),1,fp);


    ////////////////////////////////////////////////////////////JD

    readDoubleVector(&originalVS1_Jingdu, fp);
    readDoubleVector(&originalVS2_Jingdu, fp);
    readDoubleVector(&originalFlow_Jingdu, fp);
    readDoubleVector(&adjustVS1_Jingdu, fp);
    readDoubleVector(&adjustVS2_Jingdu, fp);
    readDoubleVector(&adjustFlow_Jingdu, fp);
    readDoubleVector(&b0_Jingdu, fp);
    readDoubleVector(&r0_Jingdu, fp);
    readDoubleVector(&differential0_Jingdu, fp);
    readDoubleVector(&a0_Jingdu, fp);
    readDoubleVector(&accuracy0_Jingdu, fp);
    readDoubleVector(&b1_Jingdu, fp);
    readDoubleVector(&r1_Jingdu, fp);
    readDoubleVector(&differential1_Jingdu, fp);
    readDoubleVector(&a1_Jingdu, fp);
    readDoubleVector(&accuracy1_Jingdu, fp);

    //备注
    readStringVector(&status_Jingdu, fp);
    readStringVector(&date_Jingdu, fp);
    readStringVector(&worker_Jingdu, fp);

    fread(&length_Jingdu, sizeof(int),1,fp);

    //===========================================================
    //  精度调试
    //===========================================================
    fread(&Threshold1, sizeof(double), 1, fp);
    fread(&Threshold2, sizeof(double), 1, fp);
    fread(&range1, sizeof(double), 1, fp);
    fread(&range2, sizeof(double), 1, fp);
    fread(&range3, sizeof(double), 1, fp);
    fread(&range4, sizeof(double), 1, fp);
    fread(&initValue_VS1_modeJingdu, sizeof(double), 1, fp);
    fread(&initValue_VS2_modeJingdu, sizeof(double), 1, fp);
    fread(&initValue_Yinliu_modeJingdu, sizeof(double), 1, fp);
}

void AllData::readDoubleVector(QVector<double> *vec, FILE *fp){
    double temp;
    int sum = 0;
    int len;
    sum += fread(&len, sizeof(int),1,fp);
    while(len--){
        sum += fread(&temp, sizeof(double),1,fp);
        vec->push_back(temp);
    }
}

void AllData::readStringVector(QVector<QString> *vec, FILE *fp){
    QString temp;
    int sum = 0;
    int len, l;
    sum += fread(&len, sizeof(int),1,fp);
    for(int i=0;i<len;i++){
        fread(&l, sizeof(int),1,fp);
        QChar *da = new QChar[l+1];
        fread(da, sizeof(QChar),l,fp);
        temp.setRawData(da, l);
        vec->push_back(temp);
    }
}
