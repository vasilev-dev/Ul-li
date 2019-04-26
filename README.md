"# UL_LI"
Замена в html-разметке последовательности повторяющихся тегов на конструкцию разметки списка (```html<ul><li></li></ul>```)

Пример: Заменить повторяющиеся теги ```html<p>``` в html-коде
```html<div>
 <p>Строка 1</p>
 <p>Строка 2</p>
</div>```
Выход:
```html<div>
 <ul>
 <li>Строка 1</li>
 <li>Строка 2</li>
 </ul>
</div>```
Теги считаются повторяющимися, если они следуют подряд и находятся на одном уровне вложенности.
Учесть, что заменяемые теги могут быть вложены друг в друга.
