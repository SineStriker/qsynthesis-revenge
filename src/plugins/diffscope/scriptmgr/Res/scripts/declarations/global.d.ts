declare namespace $ds {

    interface TextBox {
        type: 'TextBox';
        label: string;
        defaultValue?: string;
    }
    
    interface TextArea {
        type: 'TextArea';
        label: string;
        defaultValue?: string
    }

    interface NumberBox {
        type: 'NumberBox';
        label: string;
        precision?: number;
        min?: number;
        max?: number;
        defaultValue?: number;
    }
    
    interface Slider {
        type: 'Slider';
        label: string;
        precision?: number;
        min: number;
        max: number;
        defaultValue?: number;
    }
    
    interface KeyNameBox {
        type: 'KeyNameBox';
        label: string;
        min?: number;
        max?: number;
        defaultValue?: number;
    }
    
    interface TimeBox {
        type: 'TimeBox';
        label: string;
        format: 'MBT'|'HMS';
        relative?: boolean;
        defaultValue?: number;
    }
    
    interface CheckBox {
        type: 'CheckBox';
        label: string;
        defaultValue?: boolean;
    }
    
    interface Select {
        type: 'Select';
        label: string;
        options: string[];
        defaultValue?: number;
    }
    
    interface Label {
        type: 'Label';
        label: string;
    }
    
    type FormWidget = TextBox | TextArea | NumberBox | Slider | KeyNameBox | TimeBox | CheckBox | Select | Label;
    
    type FormReturnType<T> = T extends TextBox ? string:
                         T extends TextArea ? string:
                         T extends NumberBox ? number:
                         T extends Slider ? number:
                         T extends KeyNameBox ? number:
                         T extends TimeBox ? number:
                         T extends CheckBox ? boolean:
                         T extends Select ? number:
                         T extends Label ? null:
                         never;

    namespace dialogSystem {
        function alert(title: string, message: string): void;
        function confirm(title: string, message: string, defaultButton: 'Yes'|'No'): boolean;
        function prompt(title: string, message: string): string;
        function form<T extends $ds.FormWidget[]>(title: string, widgets: T): {
            result: 'Ok'|'Cancel';
            form: {[K in keyof T]: $ds.FormReturnType<T[K]>};
        };
    }

    namespace actionSystem {

    }

    namespace project {

    }

    namespace utils {

    }

    abstract class _ScriptBase {

        public tr(text: string): string;

        public abstract locale(lang: string, text:string): string | undefined | null;

        public abstract info(): {
            id: string;
            name: string;
            role: string | string[]
            requiredEditorVersion: string;
        };

    }

    abstract class Script extends _ScriptBase {

        public abstract main(): void;

    }

    abstract class ScriptSet extends _ScriptBase {

        public abstract main(index: number): void;

        public abstract info(): {
            id: string;
            name: string;
            role: string | string[]
            requiredEditorVersion: string;
            children: {id: string, name: string}[];
        };
    
    }

    function register(script: $ds._ScriptBase): void;
}
