-- Note (Paladin Marcus)

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('do_notemarcus', 'item', 'Note','Notiz','Notiz');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('do_notemarcus', 'item_desc', 'I''ve found this note nailed to a post in the Howling Caverns.','Diese Notiz habe ich in den Heulenden Höhlen gefunden, an einen Pfosten genagelt.','Die Notiz hani anen Pfoste gnaglet gfunde, ide Hüülende Höhle.');
INSERT INTO item_document_page(item_id, page_nr, content) values ('do_notemarcus', 1, 'NoteMarcus');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('NoteMarcus', 'document', 'To whoever reads this: Beware!

These caverns are full of traps, I only made it with a lot of luck to where I am now. The five levers are designed to kill you if you pull the wrong one. Use the last one first and the first one last and leave the others untouched.

I hope I''ll find a way out of this cave...

-- Paladin Marcus','Wer das auch immer liest: Nehmt Euch in Acht!

Diese Höhlen sind gspickt mit Fallen, ich habe es nur mit viel Glück da hin geschafft, wo ich jetzt bin. Die fünf Hebel werden Euch töten wenn Ihr den falschen zieht. Benutzt den letzten als erstes und den ersten als letztes und berührt keinen der anderen.

Ich hoffe, ich komme irgendwie wieder aus diesen Höhlen hinaus...

-- Paladin Marcus','Wer das auch immer lese tuet: Passed uuf!

Die Höhle sind voll mit Fallene, ich has nur mit viel Glück det ane gschafft, woni ez bin. Di füf Hebel werded jede töte, wo am falsche ziet. Druck de letzt als ersts und de erst als letzts und langed di andere ja nid ah!

Ich hoff, ich chum irgendwie wider us dene Höhlene use...

-- Paladin Marcus');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('do_notemarcus', 'document', 400, 400, -1);
INSERT INTO levelitem (item_id, sprite_offset_left, sprite_offset_top, bounding_box_width, bounding_box_height) values ('do_notemarcus', -25, 0, 50, 50);
INSERT INTO levelitem_frame (item_id, frame_nr, frame_left, frame_top, frame_width, frame_height) values ('do_notemarcus', 1, 450, 0, 100, 50);

-- Bloodstained Note (Paladin Marcus)

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('do_bloodnotemarcus', 'item', 'Bloodstained Note','Blutverschmierte Notiz','Bluetverschmierti Notiz');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('do_bloodnotemarcus', 'item_desc', 'Paladin Marcus must have scribbled those words on this sheet of paper before he was killed.','Paladin Marcus muss das hingekritzelt haben, bevor er getötet wurde.','De Paladin Marcus mun das anekritzlet ha bevor er töted worde isch.');
INSERT INTO item_document_page(item_id, page_nr, content) values ('do_bloodnotemarcus', 1, 'BloodNoteMarcus');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('BloodNoteMarcus', 'document', 'The beast! Transforms...

 Humans into wolves
 
 Look away!!
 They keep coming
		surrounded, I''ll fight.','Das Biest!
Verwandelt...

 Menschen in Wölfe
 
	Schau weg!!
	Es kommen immer mehr
		eingekreist, Ich muss kämpfen.'
 ,'Es Biest!
 Verwandlet...
 
 Mensche i Wölf

	Lueg weg!!
	Es chömed immer me
		umzingled, ich mun kämpfe.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('do_bloodnotemarcus', 'document', 450, 400, -1);

-- Letter (Elementalists Letter)

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('do_eleletter', 'item', 'Letter','Brief','Brief');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('do_eleletter', 'item_desc', 'I found this letter in the old travel bag that belonged to me once.','Ich habe diesen Brief in dem alten Reiserucksack gefunden, der einmal mir gehört hat.','Ich ha de Brief im alte Reiserucksack gfunde, wo mal mir ghört het.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('do_eleletter', 'document', 500, 300, -1);

-- Map (Elementalists Map)

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('do_elemap', 'item', 'Map','Karte','Charte');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('do_elemap', 'item_desc', '...','...','...');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('do_elemap', 'document', 550, 400, -1);

-- Rhendal's Diary

INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('do_elderdiary', 'item', 'Rhendal''s Diary','Rhendals Tagebuch','Es Tagebuech vom Rhendal');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('do_elderdiary', 'item_desc', 'It is disrespectful to open a stranger''s diary.','Es ist unhöflich, ein fremdes Tagebuch zu öffnen.','Es isch unhöflich, imne fremde Tagebuech z''nuusche.');
INSERT INTO item_document_page(item_id, page_nr, title) values ('do_elderdiary', -1, 'ElderDiaryTitle');
INSERT INTO item_document_page(item_id, page_nr, title, content) values ('do_elderdiary', 0, 'ElderDiaryTitle1', 'ElderDiaryContent1');
INSERT INTO item_document_page(item_id, page_nr, title, content) values ('do_elderdiary', 1, 'ElderDiaryTitle2', 'ElderDiaryContent2');
INSERT INTO item_document_page(item_id, page_nr, title, content) values ('do_elderdiary', 2, 'ElderDiaryTitle3', 'ElderDiaryContent3');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('ElderDiaryTitle', 'document', 'Diary', 'Tagebuch', 'Tagebuech');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('ElderDiaryTitle1', 'document', 'Day 212, 1103', 'Tag 212, 1103', 'Tag 212, 1103');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('ElderDiaryContent1', 'document', 'My dear Elona has still not returned. It''s unusual that she stays in the Stone Garden for such a long time. Maybe she found a new place where rare herbs grow?', 'Meine teure Elona ist immer noch nicht zurück. Es ist ungewöhnlich, dass sie so lange im Steingarten bleibt. Vielleicht hat sie eine neue Stelle gefunden, an der seltene Kräuter wachsen?', 'Mini Elona isch immer nanig heicho. Es isch ungwöhnlich, das si so lang im Steigarte bliibt. Vilicht het si e neui Stell gfunde, wo seltenigi Krütli wachsed?');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('ElderDiaryTitle2', 'document', 'Day 214, 1103', 'Tag 214, 1103', 'Tag 214, 1103');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('ElderDiaryContent2', 'document', 'I''m worried, even though she''s a good mage and can defend herself. I won''t wait any longer.

Tomorrow, I''ll start searching for her.', 'Ich mache mir Sorgen, obwohl sie eine gute Magierin ist und sich selbst verteidigen kann. Ich werde nicht noch länger hier sitzen und warten. 

Morgen werde ich nach ihr suchen.', 'Ich mach mer Sorge, obwohl si e gueti Magierin isch und sich selber verteidige cha. Ich wird nid na langer da sitze und warte. 

Morn wirdi nach ire sueche.');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('ElderDiaryTitle3', 'document', 'Day 218, 1103', 'Tag 218, 1103', 'Tag 218, 1103');
INSERT INTO text (text_id, text_type, english, german, swiss_german) values ('ElderDiaryContent3', 'document', 'I''m still not able to believe that my beloved Elona is gone. I''ve found pieces of her broken staff in the Stone Garden and tried to reassemble them, but it''s not complete. 

I can''t entomb her traditionally with her staff, but only with a part of it. I''m going to keep the magic stone from the staff here, in memory of her.', 'Ich kann es immer noch nicht fassen, dass meine geliebte Elona nicht mehr an meiner Seite ist. Ich habe Stücke von ihrem Stab im Steingarten gefunden und konnte ihn aber nicht wieder zusammensetzen, es fehlt ein Stück.

Ich kann sie also nicht mit ihrem Stab begraben, wie es die Tradition will, sondern nur mit einem Teil davon. Den magischen Stein des Stabs aber werde ich hier behalten, als Erinnerung an sie.','Ich chas immer nanig versta, dass mini gliebti Elona nüm bi mir isch. Ich han Stück vo ihrem Stab im Steigarte gfunde aber hanen nid wider chöne zemesetze, es fehlt es Stück.

Ich cha si also nid mit ihrem Stab begrabe - wes Tradition wer - nur mitme Teil devo. De magisch Stei vom Stab wirdi aber bi mir phalte, als Erinnerig a si.');
INSERT INTO item (item_id, item_type, icon_left, icon_top, gold_value) values ('do_elderdiary', 'document', 500, 500, -1);