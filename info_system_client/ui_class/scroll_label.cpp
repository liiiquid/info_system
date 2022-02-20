#include "scroll_label.h"
#include <QDebug>
#include <QPainter>
#include "client.h"
#include <QTimer>
scroll_label::scroll_label(client* cli,QWidget* parent)
{
    this->cli = cli;
    this->setParent(parent);
    QObject::connect(this,&scroll_label::contact_elem_clicked,cli,&client::get_contact_info);
}

void scroll_label::paintEvent(QPaintEvent*)
{
        QPainter p(this);
        QPen pen;
        pen.setColor(QColor(0,0,0));
        pen.setWidth(5);
        p.setPen(pen);
        p.drawLine(QPointF(247,y_start),QPointF(247,y_start + length));
}

void scroll_label::add_first_control(head_label *first_control)
{
    first_control->setParent(this);
    first_control->setFixedSize(245,30);
    first_control->is_level = 1;
    int y = this->fir_level_controls.size() > 0 ? fir_level_controls[fir_level_controls.size() -1]->y() + 30 : 0;
    first_control->move(0,y);
    QObject::connect(first_control, &head_label::contact_clicked, this,&scroll_label::show_hide_elemes);
    first_control->show();
    this->fir_level_controls.push_back(first_control);
    total_len  = this->fir_level_controls.size() * 30;
    length = this->height() * this->height() / total_len;
}

void scroll_label::add_second_control(head_label *first_control, head_label *second_control)
{
     QHash<head_label*,QVector<head_label*>*> ::Iterator i;
    if( (i = this->sec_level_controls.find(first_control)) == this->sec_level_controls.end())
    {
        QVector<head_label*>* container = new QVector<head_label*>();
        sec_level_controls.insert(first_control,container);
        i = this->sec_level_controls.find(first_control);
    }
    second_control->setFixedSize(245,30);
    second_control->setParent(this);
    second_control->is_level = 2;
    int y = i.value()->size()-1 > 0 ? i.value()->at(i.value()->size()-1)->y() + 30 : 0;
    second_control->move(0, y );
    QObject::connect(second_control, &head_label::contact_clicked, this->cli,&client::get_contact_info);
    sec_level_controls[first_control]->push_back(second_control);


}

void scroll_label::show_hide_elemes(head_label* ellm)
{
    if(ellm->is_get_server == 1) {emit contact_elem_clicked(ellm);return;} // if this btn need to get data from server, emit this signal after getting the data ready call this function
    else /*is_level == 1 -- expanding the list of this first level elem mapped*/
    {
       set_location(ellm);
    }
}

void scroll_label::set_location(head_label* ellm)
{
    if(this->sec_level_controls.find(ellm) == this->sec_level_controls.end()) return; /*if no sec list assoicated with this first level, return its origin page directly*/
   QVector<head_label*>* sec_list = this->sec_level_controls[ellm];
   /*get the start position of this first level*/
   int i = -1;
   for(i = 0; i < fir_level_controls.size();i++)
   {
       if(fir_level_controls[i] == ellm) break;
   }
   /*all the controls above this first level control*/
   int y = fir_level_controls[0]->y();/*the first elem position in this list*/
   for(int j = 0; j < i;j++)
   {
       fir_level_controls[j]->move(0,y);
       y+=30;
        if(this->sec_level_controls.find(fir_level_controls[j]) == this->sec_level_controls.end()) continue;
       QVector<head_label*>* sec_list = this->sec_level_controls[fir_level_controls[j]];
       if(fir_level_controls[j]->is_open) /*if this list is in open state, then reset the position of its sub nodes*/
       {
           for(int k = 0; k < sec_list->size();k++)
           {
               sec_list->at(k)->move(0,y); y+= 30;
               sec_list->at(k)->show();
           }
       }
       else{
           for(int k = 0; k < sec_list->size();k++)
           {
               sec_list->at(k)->hide();
           }
       }
   }/*ok, we have already set all first level elem and second level elem above this first level, now let us set this first level position*/
   /*firstly,  we need to check the is_open attribute of this node*/
   ellm->move(0,y); y += 30;
   if(ellm->is_open)/*if this first level is open, set its position else skip it...*/
   {
       for(int j = 0; j < sec_list->size();j++)
       {
           sec_list->at(j)->move(0,y); y+= 30;
           sec_list->at(j)->show();
       }
   }
   else{
       for(int k = 0; k < sec_list->size();k++)
       {
           sec_list->at(k)->hide();
       }
   }

    for(int j = i+1;j<fir_level_controls.size();j++)/*set all the position of remain elemes*/
    {
        fir_level_controls[j]->move(0,y); y += 30;
        if(this->sec_level_controls.find(fir_level_controls[j]) == this->sec_level_controls.end()) continue;
         QVector<head_label*>* sec_list = this->sec_level_controls[fir_level_controls[j]];
        if(fir_level_controls[j]->is_open)
        {
            for(int k = 0; k < sec_list->size();k++)
            {
                sec_list->at(k)->move(0,y); y+= 30;
                sec_list->at(k)->show();
            }
        }
        else{
            for(int k = 0; k < sec_list->size();k++)
            {
                sec_list->at(k)->hide();
            }
        }
    }
    this->total_len = y+ 30;
    this->length = this->height() * this->height() / total_len;
    y_start = 0;
    this->update();
}

void scroll_label::add_info_elem(head_label *ellm)/*add the elements and show them again equivalent to refresh the whole*/
{
    is_info = 1;
    for(int i = 0; i < info_list_controls.size();i++)
    {
        info_list_controls[i]->move(info_list_controls[i]->x(), info_list_controls[i]->y() +30);
        info_list_controls[i]->show();
    }
    ellm->setFixedSize(245,30);
    ellm->is_level = 2;
    ellm->setParent(this);
    ellm->move(0,10);
    QObject::connect(ellm,&head_label::info_clicked,this->cli,&client::set_send_page);
    ellm->show();
    this->info_list_controls.push_back(ellm);
    this->total_len = info_list_controls[0]->y() +info_list_controls[0]->height();
    this->length = this->height() * this->height() / total_len;
    y_start = 0;
}

void scroll_label::set_scroll_page(QVector<QLabel *> *ellm)
{
    is_info = 2;
    this->info_page = ellm;
}


void scroll_label::update_list(head_label* ellm)
{
  set_location(ellm);
}

void scroll_label::wheelEvent(QWheelEvent* ev)
{
    //if(total_len < this->height()) return;
    int delta = (ev->delta()/3);
    u =total_len / this->height();


    if(u > 0)
    {
        int dy = delta *u;
        if(is_info == 0)
        {
           /*get the start position of this first level*/
           /*all the controls above this first level control*/
           /*the first elem position in this list*/
           for(int j = 0; j < fir_level_controls.size();j++)
           {
               fir_level_controls[j]->move(0,fir_level_controls[j]->y() + dy);
               if(this->sec_level_controls.find(fir_level_controls[j]) == this->sec_level_controls.end()) continue;
               QVector<head_label*>* sec_list = this->sec_level_controls[fir_level_controls[j]];
               if(fir_level_controls[j]->is_open) /*if this list is in open state, then reset the position of its sub nodes*/
               {
                   for(int k = 0; k < sec_list->size();k++)
                   {
                       sec_list->at(k)->move(0,sec_list->at(k)->y()+dy);
                   }
               }
               else{
                   for(int k = 0; k < sec_list->size();k++)
                   {
                       sec_list->at(k)->hide();
                   }
               }
           }
        }else if(is_info == 1)
        {
            for(int i = 0; i < info_list_controls.size();i++)
            {
                info_list_controls[i]->move(info_list_controls[i]->x(),info_list_controls[i]->y() + dy);
            }
        }
        else if(is_info == 2)
        {
            for(int i = 0; i < info_page->size();i++)
            {
                info_page->at(i)->move(info_page->at(i)->x(),info_page->at(i)->y() + dy);
            }
        }
        this->update();
        is_scroll = 1;
        y_start -= delta;
    }

}

void scroll_label::clear_scroll_list()
{
    for(QHash<head_label*,QVector<head_label*>*>::Iterator i = this->sec_level_controls.begin();i!=this->sec_level_controls.end();i++)
    {
        QVector<head_label*>* container = i.value();
        while(container->size() > 0)
        {
            delete container->last(); container->removeLast();
        }
        delete container;
        this->sec_level_controls.remove(i.key());
    }
}
